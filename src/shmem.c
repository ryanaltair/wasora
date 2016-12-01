/*------------ -------------- -------- --- ----- ---   --       -            -
 *  wasora shared memory routines
 *
 *  Copyright (C) 2009--2014 jeremy theler
 *
 *  This file is part of wasora.
 *
 *  wasora is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  wasora is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with wasora.  If not, see <http://www.gnu.org/licenses/>.
 *------------------- ------------  ----    --------  --     -       -         -
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#ifndef __WIN32__
 #include <sys/mman.h>
 #include <fcntl.h>
#endif

#include "wasora.h"

void *wasora_get_shared_pointer(char *name, size_t size) {
  void *pointer;
  int dangling_pid;
#ifndef __WIN32__
  int fd;
#else
  HANDLE fd;
#endif

  if ((dangling_pid = wasora_create_lock(name, 0)) != 0) {
    wasora_push_error_message("shared memory segment '%s' is being used by process %d", name, dangling_pid);
    wasora_runtime_error();
  }
  
  if (size == 0) {
    wasora_push_error_message("shared memory object '%s' has zero size", name);
    wasora_runtime_error();
  }

#ifndef __WIN32__
  umask(0);
  if ((fd = shm_open(name, O_RDWR | O_CREAT, 0666)) == -1) {
    wasora_push_error_message("'%s' opening shared memory object '%s'", strerror(errno), name);
    wasora_runtime_error();
  }

  if (ftruncate(fd, size) != 0) {
    wasora_push_error_message("'%s' truncating shared memory object '%s'", strerror(errno), name);
    wasora_runtime_error();
  }
  if ((pointer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
    wasora_push_error_message("'%s' maping shared memory object '%s'", strerror(errno), name);
    wasora_runtime_error();
  }

  close(fd);
#else
  if ((fd = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, name)) == NULL) {
    return NULL;
  }

  if ((pointer = MapViewOfFile(fd, FILE_MAP_ALL_ACCESS, 0, 0, size)) == NULL) {
    return NULL;
  }
#endif


  return pointer;
}



void wasora_free_shared_pointer(void *pointer, char *name, size_t size) {
  wasora_remove_lock(name, 0);
#ifndef __WIN32__
  munmap(pointer, size);
  shm_unlink(name);
#else
  UnmapViewOfFile(pointer);
#endif
}


sem_t *wasora_get_semaphore(char *name) {
  sem_t *semaphore;
  int dangling_pid;

  if ((dangling_pid = wasora_create_lock(name, 1)) != 0) {
    wasora_push_error_message("sempahore '%s' is being used by process %d", name, dangling_pid);
    wasora_runtime_error();
  }


#ifndef __WIN32__
  umask(0);
  if ((semaphore = sem_open(name, O_CREAT, 0666, 0)) == SEM_FAILED) {
    perror(name);
    return NULL;
  }
#else
  if ((semaphore = CreateSemaphore(NULL, 0, 1000, name)) == NULL) {
    return NULL;
  }
#endif

  return semaphore;
}



int wasora_create_lock(char *name, int sem) {
  int dangling_pid;
  struct stat dummy;
  FILE *lock_file;
  char *lock_file_name;

  // si no hay lock_dir, entonces no generamos archivos de lock 
  if (wasora.lock_dir == NULL) {
    return WASORA_RUNTIME_OK;
  }

  if (stat(wasora.lock_dir, &dummy) == -1) {
#ifndef WIN32
    umask(0);
    if (mkdir(wasora.lock_dir, 0777) == -1) {
#else
    if (mkdir(wasora.lock_dir) == -1) {
#endif
      wasora_push_error_message("creating lock directory '%s'", wasora.lock_dir);
      wasora_runtime_error();
    }
  }

  lock_file_name = malloc(BUFFER_SIZE);
  sprintf(lock_file_name, "%s/%s%s.lock", wasora.lock_dir, (sem)?("sem."):(""), (name[0]=='/')?(name+1):(name));
  umask(0022);
  if (stat(lock_file_name, &dummy) == -1) {
    if ((lock_file = fopen(lock_file_name, "w")) == NULL) {
      wasora_push_error_message("opening lock file '%s'", lock_file_name);
      wasora_runtime_error();
    }
    fprintf(lock_file, "%10d", getpid());
    fclose(lock_file);
  } else {
    if ((lock_file = fopen(lock_file_name, "r")) == NULL) {
      wasora_push_error_message("opening lock file '%s'", lock_file_name);
      wasora_runtime_error();
    }
    if (fscanf(lock_file, "%d", &dangling_pid) != 1) {
      return -1;
    }
    fclose(lock_file);
    return dangling_pid;
  }

  free(lock_file_name);

  return 0;

}

void wasora_free_semaphore(sem_t *semaphore, char *name) {
#ifndef __WIN32__
  wasora_remove_lock(name, 1);
  sem_close(semaphore);
  sem_unlink(name);
#else
  CloseHandle(semaphore);
#endif
}

void wasora_remove_lock(char *name, int sem) {
  struct stat dummy;
  char *lock_file_name;

  // si no hay lock_dir, entonces no generamos archivos de lock
  if (wasora.lock_dir == 0) {
    return;
  }

  lock_file_name = malloc(BUFFER_SIZE);
  sprintf(lock_file_name, "%s/%s%s.lock", wasora.lock_dir, (sem)?("sem."):(""), (name[0]=='/')?(name+1):(name));
  if (stat(lock_file_name, &dummy) == -1) {
//    fprintf(stderr, "advertencia: el archivo de lock '%s' fue borrado externamente.\n", lock_file_name);
  } else {
    unlink(lock_file_name);
  }

  free(lock_file_name);

  return;
}
