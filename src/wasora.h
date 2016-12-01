/*------------ -------------- -------- --- ----- ---   --       -            -
 *  wasora common framework header
 *
 *  Copyright (C) 2009--2016 jeremy theler
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

#ifndef _WASORA_H_
#define _WASORA_H_

#include <stdio.h>
#ifndef __WIN32__
 #include <fcntl.h>
 #include <semaphore.h>
#else
 #include <windows.h>
 #define sem_t    HANDLE
#endif

#include "thirdparty/uthash.h"
#include "thirdparty/utlist.h"


#include <gsl/gsl_spline.h>
#include <gsl/gsl_multifit_nlin.h>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_multiroots.h>
#include <gsl/gsl_siman.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>


#if HAVE_IDA
#include <ida/ida.h>
#include <ida/ida_dense.h>
#include <nvector/nvector_serial.h>
#endif

// cantidad de funciones internas
#define N_BUILTIN_FUNCTIONS         53
//#define N_BUILTIN_FUNCTIONS  (sizeof(builtin_function)/sizeof(struct builtin_function_t))

// cantidad de funcionales internos
#define N_BUILTIN_FUNCTIONALS       8
//#define N_BUILTIN_FUNCTIONALS  (sizeof(builtin_functional)/sizeof(struct builtin_functional_t))

// cantidad de funciones sobre vectores
#define N_BUILTIN_VECTOR_FUNCTIONS  8

// macro para acceder al valor de una variable
#define wasora_var(var)        (*(var->value))
#define wasora_value(var)      (*(var->value))
#define wasora_var_value(var)  (*(var->value))
#define wasora_value_ptr(var)  (var->value)

// macro para acceder a las variables internas
#define wasora_special_var(var) (wasora.special_vars.var)

// macros para facilitar las sumas
#define gsl_vector_add_to_element(vector,i,x)    gsl_vector_set((vector),(i),gsl_vector_get((vector),(i))+(x))
#define gsl_matrix_add_to_element(matrix,i,j,x)  gsl_matrix_set((matrix),(i),j,gsl_matrix_get((matrix),(i),(j))+(x))

// macro para verificar errores en llamadas a funciones
#define wasora_call(function)   if ((function) != WASORA_RUNTIME_OK) return WASORA_RUNTIME_ERROR

extern const char operators[];
extern const char factorseparators[];

// tamanios maximos del buffer para un token 
#define BUFFER_SIZE        255

// delimitadores de tokens para el parser 
#define UNQUOTED_DELIM    " \t\n"
#define QUOTED_DELIM      "\""

#ifndef __WIN32__
 #define DLLAPI 
#else
 #ifndef DLL_EXPORT
  #define DLLAPI __declspec(dllexport)
 #else
  #define DLLAPI __declspec(dllimport)
 #endif
#endif

extern DLLAPI const char operators[];
extern DLLAPI const char factorseparators[];

// tipo de componentes del phase space (los numeros son de la sundials), no cambiar 
#define DAE_ALGEBRAIC                      0.0
#define DAE_DIFFERENTIAL                   1.0

// tipos de ecuaciones (lo de arriba se refiere a variables) 
// esto tiene aires de ser una mascara 
#define EQN_FLAG_GENERIC                   0   
#define EQN_FLAG_DIFFERENTIAL              1
#define EQN_FLAG_ALGEBRAIC                 2
#define EQN_FLAG_SCALAR                    4
#define EQN_FLAG_VECTORIAL                 8
#define EQN_FLAG_MATRICIAL                16
#define EQN_FLAG_IMPLICIT                 32
#define EQN_FLAG_EXPLICIT                 64

#define STEP_ALL                           0
#define STEP_BEFORE_DAE                    1
#define STEP_AFTER_DAE                     3

// defaults razonables 
#define DEFAULT_PRINT_FORMAT               "%e"
#define DEFAULT_PRINT_SEPARATOR            "\t"

#define DEFAULT_M4_FORMAT                  "%g"

#define DEFAULT_INTERPOLATION              (*gsl_interp_linear)

#define DEFAULT_RANDOM_METHOD              gsl_rng_knuthran2002

#define DEFAULT_NLIN_FIT_METHOD            gsl_multifit_fdfsolver_lmsder
#define DEFAULT_NLIN_FIT_MAX_ITER          100
#define DEFAULT_NLIN_FIT_EPSREL            1e-4
#define DEFAULT_NLIN_FIT_EPSABS            1e-6
#define DEFAULT_NLIN_FIT_GRAD_H            1e-2

#define DEFAULT_SOLVE_METHOD               gsl_multiroot_fsolver_dnewton
#define DEFAULT_SOLVE_EPSREL               0   // cero quiere decir que no mire deltas en derivadas
#define DEFAULT_SOLVE_EPSABS               1e-6
#define DEFAULT_SOLVE_MAX_ITER             1024

#define DEFAULT_MINIMIZER_METHOD           gsl_multimin_fminimizer_nmsimplex2
#define DEFAULT_MINIMIZER_TOL              1e-1
#define DEFAULT_MINIMIZER_GRADTOL          1e-3
#define DEFAULT_MINIMIZER_MAX_ITER         100
#define DEFAULT_MINIMIZER_POPULATION       20
#define DEFAULT_MINIMIZER_STEPS            5
#define DEFAULT_MINIMIZER_F_STEP           1
#define DEFAULT_MINIMIZER_FDF_STEP         1e-2
#define DEFAULT_MINIMIZER_GRAD_H           1e-2


#define DEFAULT_ROOT_MAX_TER               1024
#define DEFAULT_ROOT_TOLERANCE             (9.5367431640625e-7)  // (1/2)^-20

#define DEFAULT_INTEGRATION_INTERVALS      1024
#define DEFAULT_INTEGRATION_TOLERANCE      (9.765625e-4)         // (1/2)^-10
#define DEFAULT_INTEGRATION_KEY            GSL_INTEG_GAUSS31

#define DEFAULT_DERIVATIVE_STEP            (9.765625e-4)         // (1/2)^-10

#define DEFAULT_MULTIDIM_INTERPOLATION_THRESHOLD   (9.5367431640625e-07) // (1/2)^-20
#define DEFAULT_SHEPARD_RADIUS                     (1.0)
#define DEFAULT_SHEPARD_EXPONENT                   (2)

#define MINMAX_ARGS       10

// infinito y cero 
#define INFTY         (1125899906842624.0)                    // 2^50
#define ZERO          (8.881784197001252323389053344727e-16)  // (1/2)^-50

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *  global structures
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *  forward type definitions
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
typedef struct var_t var_t;
typedef struct vector_t vector_t;
typedef struct matrix_t matrix_t;
typedef struct alias_t alias_t;

typedef struct phase_object_t phase_object_t;
typedef struct dae_t dae_t;

typedef struct file_t file_t;
typedef struct loadable_routine_t loadable_routine_t;

typedef struct expr_t expr_t;
typedef struct factor_t factor_t;

typedef struct function_t function_t;

typedef struct instruction_t instruction_t;
typedef struct conditional_block_t conditional_block_t;

typedef struct history_t history_t;
typedef struct io_t io_t;
typedef struct io_thing_t io_thing_t;
typedef struct assignment_t assignment_t;
typedef struct call_t call_t;
typedef struct print_t print_t;
typedef struct print_token_t print_token_t;
typedef struct print_function_t print_function_t;
typedef struct print_vector_t print_vector_t;
typedef struct m4_t m4_t;
typedef struct m4_macro_t m4_macro_t;
typedef struct shell_t shell_t;
typedef struct solve_t solve_t;


typedef struct builtin_function_t builtin_function_t;
typedef struct builtin_vectorfunction_t builtin_vectorfunction_t;
typedef struct builtin_functional_t builtin_functional_t;

typedef struct multidim_range_t multidim_range_t;

// -- expresion algebraic ------------ -----        ----           --     -
// una expresion algebraica consiste en una cantidad de tokens y 
// un arreglo de tokens que la representa 
struct expr_t {
  int n_tokens;
  factor_t *token;
  double value;
  
  // por si acaso nos guardamos el string
  char *string;

  expr_t *next;
};


// -- variable -------- ------------- -----        ----           --     -
struct var_t {
  char *name;
  int initialized;
  
  // apuntadores para que alguien pueda tirar el holder
  // y hacer apuntar la variable a otro lado (por ejemplo variables
  // que estan en commons de fortran, si, asi de weird puede ser la vida)
  double *value;
  double *initial_static;
  double *initial_transient;

  // TODO: otros tipos

  // flag para saber si es hay que evaluarla en cada paso o queda constante
  int constant;
  
  // flag para saber si el apuntador de arriba lo alocamos nosotros o alguien mas
  int realloced;

  UT_hash_handle hh;
};


// -- vector ------------ -----        ----           --     -
struct vector_t {
  char *name;
  int initialized;
  
  expr_t *size_expr; // el apuntador es porque quiero hacerlo en define_vector en lugar del parser.c 
  int size;
  int constant;

  gsl_vector *value;
  gsl_vector *initial_transient;
  gsl_vector *initial_static;

  // flag para saber si el apuntador de arriba lo alocamos nosotros o alguien mas
  int realloced;
  
  // funcion para sacarle los datos
  function_t *function;
  
  // linked list con las expresiones de datos
  expr_t *datas;

  UT_hash_handle hh;
};


// -- matriz ------------ -----        ----           --     -
struct matrix_t {
  char *name;
  int initialized;  
  expr_t *cols_expr;
  expr_t *rows_expr;
  
  int cols;
  int rows;
  int constant;
  
  gsl_matrix *value;
  gsl_matrix *initial_transient;
  gsl_matrix *initial_static;

  // flag para saber si el apuntador de arriba lo alocamos nosotros o alguien mas
  int realloced;
  
  expr_t *datas;
  
  UT_hash_handle hh;
};

// -- alias --------- -------       --------           -
struct alias_t {
  int initialized;
  
  var_t *new_variable;
  
  expr_t row;
  expr_t col;
  
  var_t *variable;
  vector_t *vector;
  matrix_t *matrix;
  
  alias_t *next;
};

struct multidim_range_t {
  int dimensions;

  expr_t *min;
  expr_t *max;
  expr_t *step;
  expr_t *nsteps;
};
  
  
// -- funcion ------------ -----        ----           --     -
/* funcion de una o mas variables
 * (puede estar definida en forma algebraica o por puntos dados en el input,
 * en un archivo de texto, o en vectores, o ser el resultado de una rutina
 * del usuario) */
struct function_t {
  char *name;
  int initialized;
  
  // puede ser:
  //   - algebraica
  //   - definida por puntos
  //       + dados en el input
  //       + dados en un archivo
  //       + dados en vectores
  //       + en una malla
  //          * una propiedad continua por materiales
  //          * definida por nodos
  //          * definida por celdas
  //  - evaluada en una rutina de usuario
  //
  enum  {
    type_undefined,
    type_algebraic,
    type_pointwise_data,
    type_pointwise_file,
    type_pointwise_vector,
    type_pointwise_mesh_property,
    type_pointwise_mesh_node,
    type_pointwise_mesh_cell,
    type_routine,
  } type;
  
  // cantidad de argumentos que toma la funcion 
  int n_arguments;
  
  // los nombres de los argumentos, se usa temporalmente para parsear y
  // despues ya no se usa
  // TODO: ver si no se puede sacar de la estructura
  char **arg_name;

  // arreglo de apuntadores a variables (ya existen) que son los argumentos 
  var_t **var_argument;


  // ----------- --------- --------- ----   --     -
  // expresion para funciones algebraicas
  expr_t algebraic_expression;


  // ---------------- ------------ -------- -- --- --
  // cantidad de pares de datos x-y para funciones point-wise
  size_t data_size;

  // arreglos con los datos
  double **data_argument;
  double *data_value;

  // ayudita por si la malla es regular
  int rectangular_mesh;
  expr_t expr_x_increases_first;
  int x_increases_first;
  expr_t *expr_rectangular_mesh_size;
  int *rectangular_mesh_size;
  double **rectangular_mesh_point;

  // archivo con los datos point-wise
  char *data_file;

  // columnas donde hay que ir a buscar los datos 
  // array de tamanio n_arguments+1 (la ultima es el valor) 
  int *column;

  // vectores con los datos point-wise 
  vector_t **vector_argument;
  vector_t *vector_value;

  
  // apuntadores para interpolar con 1D gsl 
  gsl_interp *interp;
  gsl_interp_accel *interp_accel;
  gsl_interp_type interp_type;

  // flag para saber que interpolacion multidimensional se selecciona 
  enum {
    nearest,
    shepard,
    modified_shepard,
    rectangle
  } multidim_interp;

  
  expr_t expr_multidim_threshold;
  double multidim_threshold;
  expr_t expr_shepard_radius;
  double shepard_radius;
  expr_t expr_shepard_exponent;
  double shepard_exponent;

  // propiedad
  void *property;

  // malla no-estructurada sobre la que esta definida la funcion
  void *mesh;
  
  // apuntador a un arbol k-dimensional para nearest neighbors 
  void *kd;
  
  // ----- ------- -----------   --        -       - 
  // funcion que hay que llamar para funciones tipo usercall 
  double (*routine)(const double *);

  UT_hash_handle hh;
};

// archivo (puede se de entrada o de salida)
struct file_t {
  char *name;
  int initialized;
  
  char *format;
  int n_args;
  expr_t *arg;
  char *mode;
  int do_not_open;

  char *path;
  FILE *pointer;

  UT_hash_handle hh;
};

struct loadable_routine_t {
  char *name;
  int initialized;
    
  double (*routine)(const double *);
// TODO: acordarse de este para poder hacer dlclose
//  void *library;

  UT_hash_handle hh;
};

// -- token algebraic ------------ -----        ----           --     -
// un token es un parentesis, un operador, una constante, una variable o una funcion 
struct factor_t {
// constante, variable, funcion, valor inicial, etc
  int type;
// nivel de parentesis 
  int level;
// nivel temporal de parentesis, para sumas parciales 
  int tmp_level;

// numero identificatorio del operador, si es el caso 
  int oper;

// numerito de la constante numerica, si es el caso 
  double constant;

// valor actual 
  double value;
// vector con cosas auxiliares si es que se necesitan (valor anterior,
//   acumulador de la integral, etc, incluso el random generator)
  double *aux;

  // TODO: union
  builtin_function_t *builtin_function;
  builtin_vectorfunction_t *builtin_vectorfunction;
  builtin_functional_t *builtin_functional;
  var_t *variable;
  vector_t *vector;
  matrix_t *matrix;
  function_t *function;

  vector_t **vector_arg;
  
// variable sobre la que opera el funcional (segundo argumento)  
  var_t *functional_var_arg;

// expresiones algebraicas de los argumentos a una function (o vector)
  expr_t *arg;

};


// -- historia ------------ -----        ----           --     -

// historia de una variable metida en una funcion que se va actualizando tiempo a tiempo
struct history_t {
  int initialized;
  
  var_t *variable;
  function_t *function;
  int position;

  history_t *next;
};



// -- bloques condicionales ------ -       ----           --     -
struct conditional_block_t {
  conditional_block_t *father;
  conditional_block_t *else_of;
  
  expr_t *condition;
  
  instruction_t *first_true_instruction;
  instruction_t *first_false_instruction;
  
  int evaluated_to_true;
  int evaluated_to_false;
  
  conditional_block_t *next;
};

// -- instruccion ------------ -----        ----           --     -
struct instruction_t {
  int (*routine)(void *);
  void *argument;

  instruction_t *next;
};

// -- acoples ------------ -----        ----           --     -

// semaforo 
struct semaphore_t {
  char *name;
  int initialized;
  
  sem_t *pointer;
  enum {
    semaphore_wait, semaphore_post
  } operation;
  
  struct semaphore_t *next;
};

// I/O 
struct io_t {
  int initialized;
    
  enum {
    io_read,
    io_write
  } direction;

  enum {
    io_undefined,
    io_shm,
    io_file_ascii,
    io_file_binary,
    io_tcp,
    io_comedi
  } type;

/*  
  enum {
    format_double,
    format_float,
    format_char
  } floating_point_format;
*/
  int size;
  int n_things;
  
  int ignorenull;

  io_thing_t *things;

  char *shm_name;

  FILE *filepointer;
  file_t *file;
  
  // TODO: tcp & comedi
  double *shm_pointer_double;
  float *shm_pointer_float;
  char *shm_pointer_char;

  io_t *next;

};


struct io_thing_t {

  var_t *variable;
  vector_t *vector;
  matrix_t *matrix;
  expr_t expr;
  
  expr_t expr_row_min;
  expr_t expr_row_max;
  expr_t expr_col_min;
  expr_t expr_col_max;
  
  int row_min;
  int row_max;
  int col_min;
  int col_max;
  
  io_thing_t *next;
};

// -- funcion interna ------------ -----        ----           --     -
// funcion interna, se inicializa en el data space desde builinfunctions.h 
struct builtin_function_t {
  char name[BUFFER_SIZE];
  int min_arguments;
  int max_arguments;
  double (*routine)(struct factor_t *);
};


// -- funcion interna de vectores ------------ -----        ----           --     -
// funcion sobre vectores interna, se inicializa en el data space desde builinfunctions.h 
struct builtin_vectorfunction_t {
  char name[BUFFER_SIZE];
  int min_arguments;
  int max_arguments;
  double (*routine)(struct vector_t **);
};

// -- funcional interno ------------ -----        ----           --     -

// funcional interno, se inicializa en el data space
// un funcional es como una funcion pero su segundo argumento es una
// variable (no una expresion), que se usa para calcular la expresion
// del primer argumento en varios lugares y hacer algo con esto, como
// por ejemplo derivar o integrar. Ademas se agregan mas argumentos.
// i.e. integral(1+x,x,0,1)
// ie.e derivative(1+x,x,0.5)
struct builtin_functional_t {
  char name[BUFFER_SIZE];
  int min_arguments; // contando la variable del segundo argumento
  int max_arguments;
  double (*routine)(struct factor_t *, struct var_t *);
};




// -- asignacion de variables ------------ -----        ----           --     -

// igualacion algebraica 
struct  assignment_t {
  int initialized;
  
  var_t *variable;
  vector_t *vector;
  matrix_t *matrix;
  
  // expresiones que indican el rango de tiempo
  expr_t t_min;
  expr_t t_max;

  // expresiones que indican el rango de indices para asignaciones vectoriales/matriciales
  expr_t i_min;
  expr_t i_max;
  expr_t j_min;
  expr_t j_max;

  // expresiones que indican la fila y la columna en funcion de i y de j
  expr_t row;
  expr_t col;
  
  // to sum up:
  // a(row, col)<i_min,i_max;j_min,j_max>[t_min,tmax]
  
  // la expresion del miembro derecho
  expr_t rhs;
  
  // flag que indica si el assignment pide _init o _0
  int initial_static;
  int initial_transient;
  
  // si este es true entonces la asignacion es una sola (aun cuando el miembro
  // izquierdo sea un vector o matriz), i.e.
  // A(127,43)  es escalar
  // A(2*i+1,14) no es escalar
  int scalar;

  // si esto es true, entonces las asignaciones vector o matriz son plain, i.e.
  // A(i,j)
  int plain;
  
  // diferenciamos, en caso de las matrices, aquellas dependencias solo en i o en j
  // A(i,4) = 5 es un assignment que depende solo de i
  int expression_only_of_i;
  
  // A(8,j) = 1 es un assignment que depende solo de j
  int expression_only_of_j;
  
  
  assignment_t *next;

};


// -- user call ------------ -----        ----           --     -

// llamada a funcion del usuario como instruccion
struct call_t {
  char *name;
  double (*function)(const double *);

  int n_arguments;
  expr_t *arguments;

  double *x;

  call_t *next;
};


// -- print ------------ -----        ----           --     -

// instruccion imprimir linea 
struct print_t {
  // apuntador al archivo de salida 
  file_t *file;

  // una linked list con las cosas a imprimir
  print_token_t *tokens;
 
  // separador de cosas "\t" 
  // por ahora uno solo para todo el print 
  char *separator;

  // flag para indicar si hay que mandar el \n al final (es no newline asi el
  // default es cero y le manda el enter como loco de una) 
  int nonewline;

  // cosas para hacer skips 
  int last_static_step;
  int last_step;
  double last_time;

  expr_t skip_static_step;
  expr_t skip_step;
  expr_t skip_time;

  int header;
  expr_t skip_header_step;
  int last_header_step;
  int header_already_printed;
  
  print_t *next;
};


struct print_token_t {
  char *format;
  char *text;
  expr_t expression;
  
  vector_t *vector;
  matrix_t *matrix;
  function_t *function;
  
  print_token_t *next;
};


// instruccion imprimir una o mas funciones 
struct print_function_t {
  // apuntador al archivo de salida 
  file_t *file;

  // linked list con las cosas a imprimir
  print_token_t *tokens;

  // apuntador a la primera funcion que aparezca (la que tiene la cantidad posta
  // de argumentos), no es igual a token->function porque esa puede ser NULL
  function_t *first_function;

  // rango explicito donde hay que imprimir la funcion
  multidim_range_t range;

  // flag para saber que estamos imprimiendo
  int header;
  // formato de los numeritos "%e"
  char *format;
  // separador de cosas "\t"
  char *separator;

  print_function_t *next;

};

// instruccion imprimir uno o mas vectores 
struct print_vector_t {
  // apuntador al archivo de salida 
  file_t *file;

  // una linked list con las cosas a imprimir
  print_token_t *tokens;

  // apuntador a la primera funcion que aparezca (la que tiene la cantidad posta
  // de argumentos), no es igual a first_token->function porque esa puede ser NULL
  vector_t *first_vector;
  
  // formato de los numeritos "%e" 
  char *format;
  // separador de cosas "\t" 
  char *separator;
  
  int horizontal;
  expr_t elems_per_line;

  print_vector_t *next;
};

// -- m4 ------------ -----        ----           --     -
// instruccion para llamar a m4
struct m4_t {
  
  file_t *input_file;
  file_t *output_file;

  m4_macro_t *macros;
 
  m4_t *next;
};

struct m4_macro_t {
  char *name;
  print_token_t print_token;
    
  m4_macro_t *next;
};

// -- shell ------------ -----        ----           --     -
// ejecutamos un comando 
struct shell_t {
  char *format;
  int n_args;
  expr_t *arg;  

  shell_t *next;
};

// -- solve ------------ -----        ----           --     -
// resolvemos sistemas de ecuaciones no lineales
struct solve_t {
  int n;
  var_t **unknown;
  expr_t *residual;
  
  expr_t *guess;
  
  expr_t epsabs;
  expr_t epsrel;
  int max_iter;
  int verbose;

  const gsl_multiroot_fsolver_type *type;
  
  solve_t *next;  
};

// -- parametric ------------ -----        ----           --     -
// estudio parametrico 
typedef struct {
  int dimensions;
  
  enum {
    parametric_linear,
    parametric_logarithmic,
    parametric_random,
    parametric_gaussianrandom,
    parametric_sobol,
    parametric_niederreiter,
    parametric_halton,
    parametric_reversehalton
  } type;
  
  var_t **variable;
  
  // rango donde debe evaluarse la funcion a ajustar
  multidim_range_t range;
  // rangos evaluados
  double *min;
  double *max;
  double *step;
  int *nsteps;
  
  // numero total de pasos
  int outer_steps;
  
  // offset para reusar calculos ya hechos
  int offset;
  
  // lo tenemos que hacer en paralelo?
  int max_daughters;
  
  // tenemos que hacer el parametrico adiabatico?
  int adiabatic;
  
} parametric_t;


// informacion para el fit 
typedef struct {
  
  const gsl_multifit_fdfsolver_type *algorithm;
  int max_iter;
  int norerun;
  int verbose;
  
  // cantidad de parametros (i.e. a y b -> 2)
  int p;
  // cantidad de datos experimentales a ajustar (i.e. del orden de 1000)
  int n;
  
  // apuntador a la funcion cuyos parametros hay que ajustar
  function_t *function;
  // apuntador a la funcion point-wise que tiene los datos experimentales
  function_t *data;
  
  // arreglo de apuntadores a las variables que funcionan como parametros
  // y a sus incertezas estadisticas resultantes
  var_t **param;
  var_t **sigma;

  // arreglo de tamanio n_params con las expresiones de la derivada de 
  // la funcion function con respecto a los parametros
  // si es NULL hay que calcularlo a manopla numericamente
  expr_t *gradient;  
  
  // lo mismo pero para el guess inicial
  expr_t *guess;
  
  // rango donde debe evaluarse la funcion a ajustar
  multidim_range_t range;
  
  expr_t deltaepsabs;
  expr_t deltaepsrel;
  
} fit_t;


// informacion para la optimizacion 
typedef struct {
  
  int norerun;
  int verbose;
  
  // dimension de la optimizacion (ej. n = 2)
  int n;

  const gsl_multimin_fminimizer_type *f_type;
  const gsl_multimin_fdfminimizer_type *fdf_type;
  const gsl_rng_type *siman_type;
  int genetic;

  // apuntador a la funcion objetivo
  function_t *function;
  
  // o las rutinas de siman
  double (*siman_Efunc) (void *);
//  double (*siman_metric) (void *, void *);
  void *(*siman_init) (void);
  void (*siman_step) (const gsl_rng *, void *, double);
  void (*siman_copy) (void *, void *);
  void *(*siman_copy_construct) (void *);
  void (*siman_print) (void *);
  void (*siman_best) (void *);
  void (*siman_destroy) (void *);
  

  
  // arreglo de apuntadores a las variables que son los argumentos de la funcion objetivo
  var_t **x;
  // apuntador al vector que tiene el estado del solver (depende del metodo)
  gsl_vector *solver_status;
  
  // arreglo de tamanio n_params con las expresiones de la derivada de 
  // la funcion function con respecto a los parametros
  // si es NULL y el algoritmo las necesita, las tenemos que calcular
  // numericamente
  expr_t *gradient;  
  
  // lo mismo pero para el el gues inicial y los steps
  expr_t *guess;
  multidim_range_t range;

  int n_steps;

  expr_t max_iter;
  expr_t tol;
  expr_t gradtol;
} min_t;

// esto es porque getprocaddress de winkk da warnings
#ifdef __WIN32__
typedef char *(*winplugin_name_t)(void);
typedef char *(*winplugin_version_t)(void);
typedef char *(*winplugin_longversion_t)(void);
typedef char *(*winplugin_description_t)(void);
typedef char *(*winplugin_usage_t)(void);
typedef char *(*winplugin_wasorahmd5_t)(void);
typedef char *(*winplugin_copyright_t)(void);
typedef int (*winplugin_init_before_parser_t)(void);
typedef int (*winplugin_parse_line_t)(char *);
typedef int (*winplugin_init_after_parser_t)(void);
typedef int (*winplugin_init_before_run_t)(void);
typedef int (*winplugin_finalize_t)(void);
#endif

// plugin dinamico
typedef struct {
#ifndef __WIN32__
  void *handle;
#else
  HMODULE handle;
#endif
  
  char *library_file;
  char *wasorahmd5_string;
  char *name_string;
  char *version_string;
  char *longversion_string;
  char *description_string;
  char *usage_string;
  char *copyright_string;

  char *(*name)(void);
  char *(*version)(void);
  char *(*longversion)(void);
  char *(*description)(void);
  char *(*usage)(void);
  char *(*wasorahmd5)(void);
  char *(*copyright)(void);
  int (*init_before_parser)(void);
  int (*parse_line)(char *);
  int (*init_after_parser)(void);
  int (*init_before_run)(void);
  int (*finalize)(void);
  
} plugin_t;

DLLAPI struct {
  int argc;            // estos son los apuntadores a los del sistema
  char **argv;         // que getopt cambia de orden y demas
  int optind;

  int argc_orig;       // estos son copias de los originales
  char **argv_orig;    // antes de llamar a getopt

  int argc_unknown;    // estos son los que get_opt no conoce
  char **argv_unknown; // y se los pasamos a los plugins
  
  
  char *main_input_dirname_aux;
  char *main_input_dirname;
  char *main_input_basename;

  parametric_t parametric;
  fit_t fit;
  min_t min;
  
  plugin_t *plugin;
  
  var_t *vars;
  vector_t *vectors;
  matrix_t *matrices;
  alias_t *aliases;
  function_t *functions;
  file_t *files;
  loadable_routine_t *loadable_routines;

  print_t *prints;
  print_function_t *print_functions;
  print_vector_t *print_vectors;

  struct semaphore_t *semaphores;
  io_t *ios;
  assignment_t *assignments;
  call_t *calls;
  history_t *histories;
  m4_t *m4s;
  shell_t *shells;
  solve_t *solves;

  conditional_block_t *conditional_blocks;
  instruction_t *instructions;
  instruction_t *last_defined_instruction;

  struct {
    var_t *done_static;
    var_t *done_transient;
    var_t *done_outer;
    var_t *done;
  
    var_t *step_static;
    var_t *step_transient;
    var_t *step_outer;
    var_t *step_inner;

    var_t *in_outer_initial;
    var_t *in_static;
    var_t *in_static_first;
    var_t *in_static_last;
    var_t *in_transient;
    var_t *in_transient_first;
    var_t *in_transient_last;

    var_t *static_steps;
    var_t *end_time;

    var_t *time;        // estos dos son alias
    var_t *t;
    var_t *dt;

    var_t *rel_error;
    var_t *min_dt;
    var_t *max_dt;
  
    var_t *i;
    var_t *j;
    var_t *pi;
    var_t *zero;
    var_t *infinite;

    var_t *ncores;
    
    var_t *on_nan;
    var_t *on_gsl_error;
    var_t *on_ida_error;
    var_t *realtime_scale;
  } special_vars;
	
  struct {
    vector_t *abs_error;
  } special_vectors;

  struct {
    file_t *stdin_;
    file_t *stdout_;
    file_t *stderr_;
  } special_files;


  enum {
    mode_normal,
    mode_ignore_debug,
    mode_debug,
    mode_single_step,
    mode_list_vars
  } mode;
  expr_t cond_breakpoint;

  // instruction pointer
  instruction_t *ip;
  instruction_t *next_flow_instruction;

  // este es una arreglo a expresiones
  expr_t *time_path;
  // este es un apuntador a la proxima expresion
  expr_t *current_time_path;
  double next_time;

  struct timeval t0;
  struct timeval t;

  char **error;
  int error_level;
  
  int implicit_none;
  int do_not_evaluate_assigns_at_parse_time;
  
  int parametric_mode;
  int fit_mode;
  int min_mode;
  
  char *lock_dir;

  
  conditional_block_t *active_conditional_block;

  int i_plugin;
  
// este es un apuntador a un buffer interno para tener memoria sobre los
// strtoken para poder parsear cosas con comillas
  char *strtok_internal;

  size_t page_size;
  size_t actual_buffer_size; 
  char *line;
  
  // funciones que manejan la evaluacion de funciones sobre mallas
  double (*interpolate_function_node)(function_t *, const double *);
  double (*interpolate_function_cell)(function_t *, const double *);
  double (*interpolate_function_property)(function_t *, const double *);

} wasora;

struct phase_object_t {
  int offset;
  int size;
  int differential;
  char *name;

  var_t *variable;
  var_t *variable_dot;
  vector_t *vector;
  vector_t *vector_dot;
  matrix_t *matrix;
  matrix_t *matrix_dot;

  phase_object_t *next;
};

struct dae_t {
  expr_t residual;
  
  vector_t *vector;
  matrix_t *matrix;
  
  expr_t expr_i_min;
  expr_t expr_i_max;
  expr_t expr_j_min;
  expr_t expr_j_max;
  
  int i_min;
  int i_max;
  int j_min;
  int j_max;
  
  int equation_type;
  
  dae_t *next;
};

struct {
  int dimension;
  // arreglo de apuntadores a los current y a las derivadas de los
  // objectos que pertenecen al phase space
  double **phase_value;
  double **phase_derivative;
  phase_object_t *phase_objects;
  
  // linked list con las ecuaciones de los residuos
  dae_t *daes;

  void *system;

  enum {
    as_provided,
    from_variables,
    from_derivatives,
  } initial_conditions_mode;

  int reading_daes;
  instruction_t *instruction;

// ojo que el tamanio de esta estructura depende de si esta
// definido HAVE_IDA o no (ojo plugins!)
#if HAVE_IDA
  N_Vector x;
  N_Vector dxdt;
  N_Vector id;
  N_Vector abs_error;
#endif

} wasora_dae;


extern DLLAPI builtin_function_t builtin_function[N_BUILTIN_FUNCTIONS];
extern DLLAPI builtin_vectorfunction_t builtin_vectorfunction[N_BUILTIN_VECTOR_FUNCTIONS];
extern DLLAPI builtin_functional_t builtin_functional[N_BUILTIN_FUNCTIONALS];



#define WASORA_PARSER_OK              0
#define WASORA_PARSER_UNHANDLED       1
#define WASORA_PARSER_ERROR          -1
#define WASORA_PARSER_OTHER          -2

#define WASORA_RUNTIME_OK             0
#define WASORA_RUNTIME_ERROR         -1
#define WASORA_RUNTIME_NAN           -2
#define WASORA_RUNTIME_GSL           -3


// no son enums porque hacemos operaciones con las mascaras de abajo
#define EXPR_OPERATOR                             0
#define EXPR_CONSTANT                             1
#define EXPR_VARIABLE                             2
#define EXPR_VECTOR                               3
#define EXPR_MATRIX                               4
// old-school numbers
//#define EXPR_NUM                                  5
#define EXPR_BUILTIN_FUNCTION                     6
#define EXPR_BUILTIN_VECTORFUNCTION               7
#define EXPR_BUILTIN_FUNCTIONAL                   8
#define EXPR_FUNCTION                             9

#define EXPR_BASICTYPE_MASK                    1023
#define EXPR_CURRENT                              0
#define EXPR_INITIAL_TRANSIENT                 2048
#define EXPR_INITIAL_STATIC                    4096


#define ON_ERROR_NO_QUIT            1
#define ON_ERROR_NO_REPORT          2


// algebra.c 
extern DLLAPI int wasora_parse_expression(const char *, struct expr_t *);
extern DLLAPI int wasora_parse_madeup_expression(char *, struct factor_t *);
extern DLLAPI int wasora_parse_factor(char *, struct factor_t *);

extern DLLAPI double wasora_evaluate_expression(struct expr_t *);
extern DLLAPI double wasora_evaluate_expression_in_string(const char *);

extern DLLAPI int wasora_count_divisions(expr_t *);

// assignment.c 
extern DLLAPI void wasora_check_initial_variable(struct var_t *);
extern DLLAPI void wasora_check_initial_vector(struct vector_t *);
extern DLLAPI void wasora_check_initial_matrix(struct matrix_t *);
extern DLLAPI int wasora_assign_scalar(struct assignment_t *, int, int);
extern DLLAPI struct var_t *wasora_get_assignment_variable(struct assignment_t *, int, int);
extern DLLAPI int wasora_get_assignment_array_boundaries(assignment_t *, int *, int *, int *, int *);
extern DLLAPI int wasora_get_assignment_rowcol(assignment_t *, int, int, int *, int *);


// builtinfunctionals.c 
extern DLLAPI double wasora_gsl_function(double, void *);

// builtinvectorfunctions 

// call.c 

// cleanup.c 
extern DLLAPI void wasora_polite_exit(int);
extern DLLAPI void wasora_free_shm(void);
extern DLLAPI void wasora_destroy_expression(struct expr_t *);
extern DLLAPI void wasora_finalize(void);

extern DLLAPI void wasora_free_function(function_t *);
extern DLLAPI void wasora_free_functions(void);
extern DLLAPI void wasora_free_var(var_t *);
extern DLLAPI void wasora_free_vars(void);
extern DLLAPI void wasora_free_vector(vector_t *);
extern DLLAPI void wasora_free_vectors(void);
extern DLLAPI void wasora_free_matrix(matrix_t *);
extern DLLAPI void wasora_free_matrices(void);



// wasora.c 
extern DLLAPI int wasora_standard_run(void);
extern DLLAPI int wasora_step(int);

// parser.c 
extern DLLAPI int wasora_allocate_objects(void);
extern DLLAPI int wasora_parse_line(char *);
extern DLLAPI int wasora_parse_main_input_file(char *);
extern DLLAPI int wasora_parse_input_file(char *, int, int);
extern DLLAPI char *wasora_get_nth_token(char *, int);

// dae.c 
extern DLLAPI int wasora_dae_init(void);
extern DLLAPI int wasora_dae_ic(void);
#if HAVE_IDA
extern DLLAPI int wasora_ida_dae(realtype, N_Vector, N_Vector, N_Vector, void *);
#else
extern DLLAPI int wasora_ida_dae(void);
#endif

// debug.c 
extern DLLAPI void wasora_debug_printerror(void);
extern DLLAPI void wasora_debug(void);
extern DLLAPI char **wasora_rl_completion(const char *, int, int);
extern DLLAPI char *wasora_rl_symbol_generator(const char *, int);
extern DLLAPI void wasora_list_symbols(void);

// dyncall.c 
typedef double (*user_func_t)(const double*);
extern DLLAPI user_func_t set_dyn_call_so(const char *, const char *);
extern DLLAPI user_func_t get_dyn_call_so(const char *);

// error.c 
extern DLLAPI void wasora_push_error_message(const char *, ...);
extern DLLAPI void wasora_pop_error_message(void);
extern DLLAPI void wasora_pop_errors(void);
extern DLLAPI void wasora_runtime_error(void);
extern DLLAPI void wasora_nan_error(void);
extern DLLAPI void wasora_gsl_handler (const char *, const char *, int, int);

// file.c 
extern DLLAPI char *wasora_evaluate_string(char *, int, expr_t *);
extern DLLAPI FILE *wasora_fopen(const char *, const char *);


// fit.c 
extern DLLAPI int wasora_fit_run();
extern DLLAPI void wasora_fit_read_params_from_solver(const gsl_vector *);
extern DLLAPI int wasora_fit_compute_f(gsl_vector *);
extern DLLAPI void wasora_fit_compute_analytical_df(gsl_matrix *);
extern DLLAPI int wasora_fit_compute_numerical_df(gsl_matrix *);
extern DLLAPI int wasora_gsl_fit_f(const gsl_vector *m, void *, gsl_vector *);
extern DLLAPI int wasora_gsl_fit_df(const gsl_vector *, void *, gsl_matrix *);
extern DLLAPI int wasora_gsl_fit_fdf(const gsl_vector *, void *, gsl_vector *, gsl_matrix *);
extern DLLAPI void wasora_fit_print_state(int iter, int status, gsl_multifit_fdfsolver *s);

// function.c 
extern DLLAPI void wasora_set_function_args(function_t *, double *);
extern DLLAPI double wasora_evaluate_factor_function(struct factor_t *);
extern DLLAPI int wasora_function_init(function_t *);
extern DLLAPI double wasora_evaluate_function(function_t *, const double *);
extern DLLAPI int wasora_structured_scalar_index(int, int *, int *, int);

extern DLLAPI double mesh_interpolate_function_node_dummy(function_t *, const double *);
extern DLLAPI double mesh_interpolate_function_cell_dummy(function_t *, const double *);
extern DLLAPI double mesh_interpolate_function_property_dummy(function_t *, const double *);


extern DLLAPI double mesh_interpolate_function_node(function_t *, const double *);
extern DLLAPI double mesh_interpolate_function_cell(function_t *, const double *);

// getptr.c 
extern DLLAPI var_t *wasora_get_variable_ptr(const char *);
extern DLLAPI vector_t *wasora_get_vector_ptr(const char *);
extern DLLAPI matrix_t *wasora_get_matrix_ptr(const char *);
extern DLLAPI function_t *wasora_get_function_ptr(const char *);
extern DLLAPI builtin_function_t *wasora_get_builtin_function_ptr(const char *);
extern DLLAPI builtin_vectorfunction_t *wasora_get_builtin_vectorfunction_ptr(const char *);
extern DLLAPI builtin_functional_t *wasora_get_builtin_functional_ptr(const char *);
extern DLLAPI file_t  *wasora_get_file_ptr(const char *);
extern DLLAPI loadable_routine_t *wasora_get_loadable_routine(const char *);
extern DLLAPI double (*wasora_get_routine_ptr(const char *))(const double *);
extern DLLAPI struct semaphore_t *wasora_get_sem_ptr(char *);
extern DLLAPI vector_t *wasora_get_first_vector(const char *);
extern DLLAPI matrix_t *wasora_get_first_matrix(const char *);
extern DLLAPI char *wasora_get_first_dot(const char *s);

// handler.c 
extern DLLAPI void wasora_signal_handler(int);

// history.c 
extern DLLAPI void wasora_init_history(history_t *);

// init.c 
extern DLLAPI int wasora_init_before_parser(void);
extern DLLAPI int wasora_init_after_parser(void);
extern DLLAPI int wasora_init_before_run(void);
extern DLLAPI int wasora_is_structured_grid_2d(double *, double *, int, int *, int *);
extern DLLAPI int wasora_is_structured_grid_3d(double *, double *, double *, int, int *, int *, int *);

// io.c 
extern DLLAPI int wasora_io_init(io_t *);
extern DLLAPI int wasora_io_read_shm(io_t *, double *, int);
extern DLLAPI int wasora_io_write_shm(io_t *, double *, int);
extern DLLAPI int wasora_io_read_ascii_file(io_t *, double *, int);
extern DLLAPI int wasora_io_write_ascii_file(io_t *, double *, int);
extern DLLAPI int wasora_io_read_binary_file(io_t *, double *, int);
extern DLLAPI int wasora_io_write_binary_file(io_t *, double *, int);


// matrix.c
extern DLLAPI double wasora_matrix_get(matrix_t *, const size_t, const size_t);
extern DLLAPI double wasora_matrix_get_initial_transient(matrix_t *, const size_t,  const size_t);
extern DLLAPI double wasora_matrix_get_initial_static(matrix_t *, const size_t,  const size_t);
extern DLLAPI int wasora_matrix_init(matrix_t *);

// minimize.c 
extern DLLAPI int wasora_min_run();
extern DLLAPI void wasora_min_read_params_from_solver(const gsl_vector *) ;
extern DLLAPI double wasora_min_compute_f(const double *);
extern DLLAPI double wasora_gsl_min_f(const gsl_vector *, void *);
extern DLLAPI void wasora_gsl_min_df(const gsl_vector *, void *, gsl_vector *);
extern DLLAPI void wasora_gsl_min_fdf(const gsl_vector *, void *, double *, gsl_vector *);

// multiminf.c 
void wasora_min_multiminf(gsl_vector *);
extern DLLAPI void wasora_multiminf_print_state(int, gsl_multimin_fminimizer *);

// multiminfdf.c 
void wasora_min_multiminfdf(gsl_vector *);
extern DLLAPI void wasora_multiminfdf_print_state(int, gsl_multimin_fdfminimizer *);

// multirootc
extern DLLAPI int wasora_gsl_solve_f(const gsl_vector *x, void *params, gsl_vector *f);

// parametric.c 
extern DLLAPI int wasora_parametric_run();
extern DLLAPI void wasora_parametric_run_parallel();


// parser.c
extern DLLAPI int wasora_parse_first_pass(const char *, int, int);

// parseaux.c 
extern DLLAPI int wasora_parser_expression(expr_t *);
extern DLLAPI int wasora_parser_expressions(expr_t *[], size_t);
extern DLLAPI int wasora_parser_expression_in_string(double *);
extern DLLAPI int wasora_parser_match_keyword_expression(char *, char *[], expr_t *[], size_t);
extern DLLAPI int wasora_parser_string(char **);
extern DLLAPI int wasora_parser_string_format(char **, int *);
extern DLLAPI int wasora_parser_file(file_t **);
extern DLLAPI int wasora_parser_file_path(file_t **, char *);
extern DLLAPI int wasora_parser_function(function_t **);
extern DLLAPI int wasora_parser_vector(vector_t **);
extern DLLAPI int wasora_parser_variable(var_t **);
extern DLLAPI int wasora_parser_keywords_ints(char *[], int *, int *);
extern DLLAPI int wasora_parse_assignment(char *, assignment_t *);

extern DLLAPI void wasora_realloc_variable_ptr(var_t *, double *, int);
extern DLLAPI void wasora_realloc_vector_ptr(vector_t *, double *, int);
extern DLLAPI void wasora_realloc_matrix_ptr(matrix_t *, double *, int);
extern DLLAPI var_t *wasora_define_variable(char *);
extern DLLAPI vector_t *wasora_define_vector(char *, int, expr_t *, expr_t *);
extern DLLAPI matrix_t *wasora_define_matrix(char *, int, expr_t *, int, expr_t *, expr_t *);
extern DLLAPI function_t *wasora_define_function(const char *, int);
extern DLLAPI file_t *wasora_define_file(char *, char *, int, expr_t *, char *, int);

extern DLLAPI loadable_routine_t *wasora_define_loadable_routine(char *, void *);
extern DLLAPI instruction_t *wasora_define_instruction(int (*)(void *), void *);
extern DLLAPI var_t *wasora_get_or_define_variable_ptr(char *);
extern DLLAPI char *wasora_get_next_token(char *);
extern DLLAPI int wasora_check_name(const char *);
extern DLLAPI int wasora_parse_range(char *, const char, const char, const char, struct expr_t *, struct expr_t *);

extern DLLAPI int wasora_parse_factor(char *, struct factor_t *);
extern DLLAPI int wasora_read_line(FILE *);
extern DLLAPI int wasora_read_data_line(FILE *, char *);
extern DLLAPI void wasora_strip_blanks(char *);
extern DLLAPI void wasora_strip_brackets(char *);
extern DLLAPI void wasora_add_leading_zeros(char **);
extern DLLAPI int wasora_replace_arguments(char **);
extern DLLAPI int wasora_count_arguments(char *);
extern DLLAPI void wasora_syntax_error(char *, int);

extern DLLAPI char *wasora_ends_in_zero(char *);
extern DLLAPI char *wasora_ends_in_init(char *);
extern DLLAPI char *wasora_ends_in_i(char *);
extern DLLAPI char *wasora_is_vector_underscore_something(char *);
extern DLLAPI char *wasora_is_matrix_underscore_something(char *, char **, char **);



// plugin.c
extern DLLAPI int wasora_load_hardcoded_plugin(void);
extern DLLAPI int wasora_load_plugin(const char *);
#ifndef __WIN32__
extern DLLAPI void *wasora_dlopen(const char *);
extern DLLAPI int wasora_dlopen_try(const char *, const char *, const char *, void **);
#else
extern DLLAPI HMODULE wasora_dlopen(const char *);
extern DLLAPI int wasora_dlopen_try(const char *, const char *, const char *, HMODULE *);
#endif  


// print.c

// randomline.c
extern DLLAPI void wasora_print_random_line(FILE *, int);

// realtime.c
extern DLLAPI void wasora_init_realtime(void);
extern DLLAPI void wasora_wait_realtime(void);

// shell.c


// shmem.c
extern DLLAPI void *wasora_get_shared_pointer(char *, size_t);
extern DLLAPI void wasora_free_shared_pointer(void *, char *, size_t);
extern DLLAPI sem_t *wasora_get_semaphore(char *);
extern DLLAPI void wasora_free_semaphore(sem_t *, char *);
extern DLLAPI int wasora_create_lock(char *, int);
extern DLLAPI void wasora_remove_lock(char *, int);

// siman.c
void wasora_min_siman(gsl_vector *);
extern DLLAPI double wasora_siman_Efunc_real(void *);
extern DLLAPI double wasora_siman_metric_real(void *, void *);
extern DLLAPI void wasora_siman_step_real(const gsl_rng *, void *, double);
extern DLLAPI void *wasora_siman_copy_construct_real(void *);
extern DLLAPI void wasora_siman_copy_real(void *, void *);
extern DLLAPI void wasora_siman_destroy_real(void *);
extern DLLAPI void wasora_siman_print_real(void *);

// vector.c
extern DLLAPI double wasora_vector_get(vector_t *, const size_t);
extern DLLAPI double wasora_vector_get_initial_transient(vector_t *, const size_t);
extern DLLAPI double wasora_vector_get_initial_static(vector_t *, const size_t);
extern DLLAPI int wasora_vector_init(vector_t *);

// version.c
extern DLLAPI void wasora_show_help();
extern DLLAPI void wasora_show_version(int);
extern DLLAPI void wasora_shortversion(void);
extern DLLAPI void wasora_longversion(void);

// str_replace.c
char *str_replace (const char *, const char *, const char *);

// instructions
extern DLLAPI int wasora_instruction_if(void *);
extern DLLAPI int wasora_instruction_else(void *);
extern DLLAPI int wasora_instruction_endif(void *);
extern DLLAPI int wasora_instruction_parametric(void *);


extern DLLAPI int wasora_instruction_alias(void *);
extern DLLAPI int wasora_instruction_sem(void *);
extern DLLAPI int wasora_instruction_io(void *);
extern DLLAPI int wasora_instruction_history(void *);
extern DLLAPI int wasora_instruction_print(void *);
extern DLLAPI int wasora_instruction_print_function(void *);
extern DLLAPI int wasora_instruction_print_vector(void *);
extern DLLAPI int wasora_instruction_print_matrix(void *);
extern DLLAPI int wasora_instruction_m4(void *);
extern DLLAPI int wasora_instruction_shell(void *);
extern DLLAPI int wasora_instruction_solve(void *);
extern DLLAPI int wasora_instruction_call(void *);
extern DLLAPI int wasora_instruction_assignment(void *);
extern DLLAPI int wasora_instruction_dae(void *);
extern DLLAPI int wasora_instruction_file(void *);
extern DLLAPI int wasora_instruction_open_file(void *);
extern DLLAPI int wasora_instruction_close_file(void *);
extern DLLAPI int wasora_instruction_abort(void *);
extern DLLAPI int wasora_instruction_mesh(void *);
extern DLLAPI int wasora_instruction_mesh_post(void *);
extern DLLAPI int wasora_instruction_mesh_fill_vector(void *);
extern DLLAPI int wasora_instruction_mesh_find_max(void *);
extern DLLAPI int wasora_instruction_mesh_integrate(void *arg);


// interface.h
// variable-related functions
extern DLLAPI int wasora_exists_var(const char *);
extern DLLAPI double wasora_get_var_value(const char *);
extern DLLAPI double *wasora_get_var_value_ptr(const char *);
extern DLLAPI void wasora_set_var_value(const char *, double);

// vector-related functions
extern DLLAPI int wasora_exists_vector(const char *);
extern DLLAPI double wasora_get_vector_value(const char *, int);
extern DLLAPI gsl_vector *wasora_get_vector_gsl_ptr(const char *);
extern DLLAPI void wasora_set_vector_value(const char *, int, double);
extern DLLAPI double *wasora_get_crisp_pointer(const char *);
extern DLLAPI int wasora_get_vector_size(const char *);

// function-related functions
extern DLLAPI int wasora_exists_function(const char *);
extern DLLAPI double wasora_get_function_value(const char *name, double *arg);

extern DLLAPI int wasora_exists_var(const char *);
extern DLLAPI double wasoraexistsvar_(const char *, int);
extern DLLAPI double wasora_get_var_value(const char *);
extern DLLAPI double wasoragetvarvalue_(const char *, int);
extern DLLAPI void wasora_set_var_value(const char *, double);
extern DLLAPI int wasorasetvarvalue_(const char *, double *, int);

extern DLLAPI int wasora_exists_vector(const char *);
extern DLLAPI double wasoraexistsvector_(const char *, int);
extern DLLAPI double wasora_get_vector_value(const char *, int);
extern DLLAPI double wasoragetvectorvalue_(const char *, int *, int);
extern DLLAPI void wasora_set_vector_value(const char *, int, double);
extern DLLAPI int wasorasetvectorvalue_(const char *, int *, double *, int);
extern DLLAPI double *wasora_get_crisp_pointer(const char *);
extern DLLAPI int wasora_get_vector_size(const char *);
extern DLLAPI int wasoragetvectorsize_(const char *, int);

extern DLLAPI int wasora_exists_matrix(const char *);
extern DLLAPI double wasoraexistsmatrix_(const char *, int);
extern DLLAPI double wasora_get_matrix_value(const char *, int , int);
extern DLLAPI gsl_matrix *wasora_get_matrix_gsl_ptr(const char *);
extern DLLAPI double wasoragetmatrixvalue_(const char *, int *, int *, int);
extern DLLAPI void wasora_set_matrix_value(const char *, int , int , double);
extern DLLAPI int wasorasetmatrixvalue_(const char *, int *, int *, double *, int);
extern DLLAPI double *wasora_get_matrix_crisp_pointer(const char *);
extern DLLAPI int wasora_get_matrix_rows(const char *);
extern DLLAPI int wasora_get_matrix_cols(const char *);
extern DLLAPI int wasoragetmatrixcols_(const char *, int);
extern DLLAPI int wasoragetmatrixrows_(const char *, int);


extern DLLAPI int wasora_exists_function(const char *);
extern DLLAPI double wasoraexistsfunction_(const char *, int);
extern DLLAPI double wasoragetfunctionvalue(const char *, double *, int);
extern DLLAPI double wasora_get_function_value(const char *name, double *arg);


#endif  /* _WASORA_H_ */

