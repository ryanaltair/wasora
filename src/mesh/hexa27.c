/*------------ -------------- -------- --- ----- ---   --       -            -
 *  wasora's mesh-related hexahedron element routines
 *
 *  Copyright (C) 2014--2017 C.P. Camusso.
 *  Copyright (C) 2017--2020 jeremy theler
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
#include <wasora.h>

#include <math.h>
#include <gsl/gsl_linalg.h>


// --------------------------------------------------------------
// hexahedro de 27 nodos
// --------------------------------------------------------------
int mesh_hexa27_init(void) {
  
  double r[3];
  element_type_t *element_type;
  int j, v;

  element_type = &wasora_mesh.element_type[ELEMENT_TYPE_HEXAHEDRON27];
  element_type->name = strdup("hexa27");
  element_type->id = ELEMENT_TYPE_HEXAHEDRON27;
  element_type->dim = 3;
  element_type->order = 2;
  element_type->nodes = 27;
  element_type->faces = 6;
  element_type->nodes_per_face = 9;
  element_type->h = mesh_hexa27_h;
  element_type->dhdr = mesh_hexa27_dhdr;
  element_type->point_in_element = mesh_point_in_hexahedron;
  element_type->element_volume = mesh_hexahedron_vol;

/*
 
  Hexahedron27:
  
  3----13----2     
  |\         |\    
  |15    24  | 14  
  9  \ 20    11 \  
  |   7----19+---6 
  |22 |  26  | 23| 
  0---+-8----1   | 
   \ 17    25 \  18
   10 |  21    12| 
     \|         \| 
      4----16----5 
 */  
  
  element_type->node_coords = calloc(element_type->nodes, sizeof(double *));
  element_type->node_parents = calloc(element_type->nodes, sizeof(node_relative_t *));  
  for (j = 0; j < element_type->nodes; j++) {
    element_type->node_coords[j] = calloc(element_type->dim, sizeof(double));  
  }

  
  element_type->first_order_nodes++;  
  element_type->node_coords[0][0] = -1;
  element_type->node_coords[0][1] = -1;
  element_type->node_coords[0][2] = -1;

  element_type->first_order_nodes++;  
  element_type->node_coords[1][0] = +1;  
  element_type->node_coords[1][1] = -1;
  element_type->node_coords[1][2] = -1;
  
  element_type->first_order_nodes++;
  element_type->node_coords[2][0] = +1;  
  element_type->node_coords[2][1] = +1;
  element_type->node_coords[2][2] = -1;

  element_type->first_order_nodes++;
  element_type->node_coords[3][0] = -1;
  element_type->node_coords[3][1] = +1;
  element_type->node_coords[3][2] = -1;
 
  element_type->first_order_nodes++;
  element_type->node_coords[4][0] = -1;
  element_type->node_coords[4][1] = -1;
  element_type->node_coords[4][2] = +1;
  
  element_type->first_order_nodes++;
  element_type->node_coords[5][0] = +1;  
  element_type->node_coords[5][1] = -1;
  element_type->node_coords[5][2] = +1;
  
  element_type->first_order_nodes++;
  element_type->node_coords[6][0] = +1;  
  element_type->node_coords[6][1] = +1;
  element_type->node_coords[6][2] = +1;

  element_type->first_order_nodes++;
  element_type->node_coords[7][0] = -1;
  element_type->node_coords[7][1] = +1;
  element_type->node_coords[7][2] = +1;
  
  wasora_mesh_add_node_parent(&element_type->node_parents[8], 0);
  wasora_mesh_add_node_parent(&element_type->node_parents[8], 1);
  wasora_mesh_compute_coords_from_parent(element_type, 8);
  
  wasora_mesh_add_node_parent(&element_type->node_parents[9], 0);
  wasora_mesh_add_node_parent(&element_type->node_parents[9], 3);
  wasora_mesh_compute_coords_from_parent(element_type, 9); 
 
  wasora_mesh_add_node_parent(&element_type->node_parents[10], 0);
  wasora_mesh_add_node_parent(&element_type->node_parents[10], 4);
  wasora_mesh_compute_coords_from_parent(element_type, 10);   
 
  wasora_mesh_add_node_parent(&element_type->node_parents[11], 1);
  wasora_mesh_add_node_parent(&element_type->node_parents[11], 2);
  wasora_mesh_compute_coords_from_parent(element_type, 11);

  wasora_mesh_add_node_parent(&element_type->node_parents[12], 1);
  wasora_mesh_add_node_parent(&element_type->node_parents[12], 5);
  wasora_mesh_compute_coords_from_parent(element_type, 12);

  wasora_mesh_add_node_parent(&element_type->node_parents[13], 2);
  wasora_mesh_add_node_parent(&element_type->node_parents[13], 3);
  wasora_mesh_compute_coords_from_parent(element_type, 13);  

  wasora_mesh_add_node_parent(&element_type->node_parents[14], 2);
  wasora_mesh_add_node_parent(&element_type->node_parents[14], 6);
  wasora_mesh_compute_coords_from_parent(element_type, 14);   
 
  wasora_mesh_add_node_parent(&element_type->node_parents[15], 3);
  wasora_mesh_add_node_parent(&element_type->node_parents[15], 7);
  wasora_mesh_compute_coords_from_parent(element_type, 15);

  wasora_mesh_add_node_parent(&element_type->node_parents[16], 4);
  wasora_mesh_add_node_parent(&element_type->node_parents[16], 5);
  wasora_mesh_compute_coords_from_parent(element_type, 16);

  wasora_mesh_add_node_parent(&element_type->node_parents[17], 4);
  wasora_mesh_add_node_parent(&element_type->node_parents[17], 7);
  wasora_mesh_compute_coords_from_parent(element_type, 17);    

  wasora_mesh_add_node_parent(&element_type->node_parents[18], 5);
  wasora_mesh_add_node_parent(&element_type->node_parents[18], 6);
  wasora_mesh_compute_coords_from_parent(element_type, 18);

  wasora_mesh_add_node_parent(&element_type->node_parents[19], 6);
  wasora_mesh_add_node_parent(&element_type->node_parents[19], 7);
  wasora_mesh_compute_coords_from_parent(element_type, 19);    

  wasora_mesh_add_node_parent(&element_type->node_parents[20], 0);
  wasora_mesh_add_node_parent(&element_type->node_parents[20], 1);
  wasora_mesh_add_node_parent(&element_type->node_parents[20], 2);
  wasora_mesh_add_node_parent(&element_type->node_parents[20], 3);
  wasora_mesh_compute_coords_from_parent(element_type, 20);    

  wasora_mesh_add_node_parent(&element_type->node_parents[21], 0);
  wasora_mesh_add_node_parent(&element_type->node_parents[21], 1);
  wasora_mesh_add_node_parent(&element_type->node_parents[21], 5);
  wasora_mesh_add_node_parent(&element_type->node_parents[21], 4);
  wasora_mesh_compute_coords_from_parent(element_type, 21);    
  
  wasora_mesh_add_node_parent(&element_type->node_parents[22], 0);
  wasora_mesh_add_node_parent(&element_type->node_parents[22], 3);
  wasora_mesh_add_node_parent(&element_type->node_parents[22], 7);
  wasora_mesh_add_node_parent(&element_type->node_parents[22], 4);
  wasora_mesh_compute_coords_from_parent(element_type, 22);    
  
  wasora_mesh_add_node_parent(&element_type->node_parents[23], 1);
  wasora_mesh_add_node_parent(&element_type->node_parents[23], 2);
  wasora_mesh_add_node_parent(&element_type->node_parents[23], 6);
  wasora_mesh_add_node_parent(&element_type->node_parents[23], 5);
  wasora_mesh_compute_coords_from_parent(element_type, 23);    

  wasora_mesh_add_node_parent(&element_type->node_parents[24], 2);
  wasora_mesh_add_node_parent(&element_type->node_parents[24], 3);
  wasora_mesh_add_node_parent(&element_type->node_parents[24], 7);
  wasora_mesh_add_node_parent(&element_type->node_parents[24], 6);
  wasora_mesh_compute_coords_from_parent(element_type, 24);    
  
  wasora_mesh_add_node_parent(&element_type->node_parents[25], 4);
  wasora_mesh_add_node_parent(&element_type->node_parents[25], 5);
  wasora_mesh_add_node_parent(&element_type->node_parents[25], 6);
  wasora_mesh_add_node_parent(&element_type->node_parents[25], 7);
  wasora_mesh_compute_coords_from_parent(element_type, 25);    

  wasora_mesh_add_node_parent(&element_type->node_parents[26], 0);
  wasora_mesh_add_node_parent(&element_type->node_parents[26], 1);
  wasora_mesh_add_node_parent(&element_type->node_parents[26], 2);
  wasora_mesh_add_node_parent(&element_type->node_parents[26], 3);
  wasora_mesh_add_node_parent(&element_type->node_parents[26], 4);
  wasora_mesh_add_node_parent(&element_type->node_parents[26], 5);
  wasora_mesh_add_node_parent(&element_type->node_parents[26], 6);
  wasora_mesh_add_node_parent(&element_type->node_parents[26], 7);
  wasora_mesh_compute_coords_from_parent(element_type, 26);    

  // full integration: 3x3x3
  mesh_gauss_init_hexa27(element_type, &element_type->gauss[integration_full]);
  element_type->gauss[integration_full].extrap = gsl_matrix_calloc(element_type->nodes, 27);

  // reduced integration: 2x2x2
  mesh_gauss_init_hexa8(element_type, &element_type->gauss[integration_reduced]);
  element_type->gauss[integration_reduced].extrap = gsl_matrix_calloc(element_type->nodes, 8);
  
  // the two extrapolation matrices
  for (j = 0; j < element_type->first_order_nodes; j++) {
    r[0] = M_SQRT3 * element_type->node_coords[j][0];
    r[1] = M_SQRT3 * element_type->node_coords[j][1];
    r[2] = M_SQRT3 * element_type->node_coords[j][2];

    for (v = 0; v < 8; v++) {
      gsl_matrix_set(element_type->gauss[integration_full].extrap, j, v, mesh_hexa8_h(v, r));
      gsl_matrix_set(element_type->gauss[integration_reduced].extrap, j, v, mesh_hexa8_h(v, r));
    }
  }
  
  return WASORA_RUNTIME_OK;
}




/*
Reference https://www.code-aster.org/V2/doc/v11/en/man_r/r3/r3.01.01.pdf

                                              Coordinates
Node here(gmsh)       Node reference        r        s        t
 0                       1                 -1       -1       -1
 1                       2                  1       -1       -1
 2                       3                  1        1       -1
 3                       4                 -1        1       -1
 4                       5                 -1       -1        1
 5                       6                  1       -1        1
 6                       7                  1        1        1
 7                       8                 -1        1        1
 8                       9                  0       -1       -1
11                      10                  1        0       -1
13                      11                  0        1       -1
 9                      12                 -1        0       -1
10                      13                 -1       -1        0
12                      14                  1       -1        0
14                      15                  1        1        0
15                      16                 -1        1        0
16                      17                  0       -1        1
18                      18                  1        0        1
19                      19                  0        1        1
17                      20                 -1        0        1
20                      21                  0        0       -1
21                      22                  0       -1        0
23                      23                  1        0        0
24                      24                  0        1        0
22                      25                 -1        0        0
25                      26                  0        0        1
26                      27                  0        0        0
*/
double mesh_hexa27_h(int j, double *vec_r) {
  double r = vec_r[0];
  double s = vec_r[1];
  double t = vec_r[2];

  switch (j) {
    case 0:
      return r*(r-1)*s*(s-1)*t*(t-1)/8.0;
      break;
    case 1:
      return r*(r+1)*s*(s-1)*t*(t-1)/8.0;
      break;
    case 2:
      return r*(r+1)*s*(s+1)*t*(t-1)/8.0;
      break;
    case 3:
      return r*(r-1)*s*(s+1)*t*(t-1)/8.0;
      break;
    case 4:
      return r*(r-1)*s*(s-1)*t*(t+1)/8.0;
      break;
    case 5:
      return r*(r+1)*s*(s-1)*t*(t+1)/8.0;
      break;
    case 6:
      return r*(r+1)*s*(s+1)*t*(t+1)/8.0;
      break;
    case 7:
      return r*(r-1)*s*(s+1)*t*(t+1)/8.0;
      break;
    case 8:
      return (1-r*r)*s*(s-1)*t*(t-1)/4.0;
      break;
    case 9:
      return r*(r-1)*(1-s*s)*t*(t-1)/4.0;
      break;
    case 10:
      return r*(r-1)*s*(s-1)*(1-t*t)/4.0;
      break;
    case 11:
      return r*(r+1)*(1-s*s)*t*(t-1)/4.0;
      break;
    case 12:
      return r*(r+1)*s*(s-1)*(1-t*t)/4.0;
      break;
    case 13:
      return (1-r*r)*s*(s+1)*t*(t-1)/4.0;
      break;
    case 14:
      return r*(r+1)*s*(s+1)*(1-t*t)/4.0;
      break;
    case 15:
      return r*(r-1)*s*(s+1)*(1-t*t)/4.0;
      break;
    case 16:
      return (1-r*r)*s*(s-1)*t*(t+1)/4.0;
      break;
    case 17:
      return r*(r-1)*(1-s*s)*t*(t+1)/4.0;
      break;
    case 18:
      return r*(r+1)*(1-s*s)*t*(t+1)/4.0;
      break;
    case 19:
      return (1-r*r)*s*(s+1)*t*(t+1)/4.0;
      break;
    case 20:
      return (1-r*r)*(1-s*s)*t*(t-1)/2.0;
      break;
    case 21:
      return (1-r*r)*s*(s-1)*(1-t*t)/2.0;
      break;
    case 22:
      return r*(r-1)*(1-s*s)*(1-t*t)/2.0;
      break;
    case 23:
      return r*(r+1)*(1-s*s)*(1-t*t)/2.0;
      break;
    case 24:
      return (1-r*r)*s*(s+1)*(1-t*t)/2.0;
      break;
    case 25:
      return (1-r*r)*(1-s*s)*t*(t+1)/2.0;
      break;
    case 26:
      return (1-r*r)*(1-s*s)*(1-t*t);
      break;
  }

  return 0;

}

double mesh_hexa27_dhdr(int j, int m, double *vec_r) {
  double r = vec_r[0];
  double s = vec_r[1];
  double t = vec_r[2];

  switch (j) {
    case 0:
      switch(m) {
        case 0:
          return (2*r-1)*s*(s-1)*t*(t-1)/8.0;
        break;
        case 1:
          return r*(r-1)*(2*s-1)*t*(t-1)/8.0;
        break;
        case 2:
          return r*(r-1)*s*(s-1)*(2*t-1)/8.0;
        break;
      }
    break;
    case 1:
      switch(m) {
        case 0:
          return (2*r+1)*s*(s-1)*t*(t-1)/8.0;
        break;
        case 1:
          return r*(r+1)*(2*s-1)*t*(t-1)/8.0;
        break;
        case 2:
          return r*(r+1)*s*(s-1)*(2*t-1)/8.0;
        break;
      }
    break;
    case 2:
      switch(m) {
        case 0:
          return (2*r+1)*s*(s+1)*t*(t-1)/8.0;
        break;
        case 1:
          return r*(r+1)*(2*s+1)*t*(t-1)/8.0;
        break;
        case 2:
          return r*(r+1)*s*(s+1)*(2*t-1)/8.0;
        break;
      }
    break;
    case 3:
      switch(m) {
        case 0:
          return (2*r-1)*s*(s+1)*t*(t-1)/8.0;
        break;
        case 1:
          return r*(r-1)*(2*s+1)*t*(t-1)/8.0;
        break;
        case 2:
          return r*(r-1)*s*(s+1)*(2*t-1)/8.0;
        break;
      }
    break;
    case 4:
      switch(m) {
        case 0:
          return (2*r-1)*s*(s-1)*t*(t+1)/8.0;
        break;
        case 1:
          return r*(r-1)*(2*s-1)*t*(t+1)/8.0;
        break;
        case 2:
          return r*(r-1)*s*(s-1)*(2*t+1)/8.0;
        break;
      }
    break;
    case 5:
      switch(m) {
        case 0:
          return (2*r+1)*s*(s-1)*t*(t+1)/8.0;
        break;
        case 1:
          return r*(r+1)*(2*s-1)*t*(t+1)/8.0;
        break;
        case 2:
          return r*(r+1)*s*(s-1)*(2*t+1)/8.0;
        break;
      }
    break;
    case 6:
      switch(m) {
        case 0:
          return (2*r+1)*s*(s+1)*t*(t+1)/8.0;
        break;
        case 1:
          return r*(r+1)*(2*s+1)*t*(t+1)/8.0;
        break;
        case 2:
          return r*(r+1)*s*(s+1)*(2*t+1)/8.0;
        break;
      }
    break;
    case 7:
      switch(m) {
        case 0:
          return (2*r-1)*s*(s+1)*t*(t+1)/8.0;
        break;
        case 1:
          return r*(r-1)*(2*s+1)*t*(t+1)/8.0;
        break;
        case 2:
          return r*(r-1)*s*(s+1)*(2*t+1)/8.0;
        break;
      }
    break;
    case 8:
      switch(m) {
        case 0:
          return -r*s*(s-1)*t*(t-1)/2.0;
        break;
        case 1:
          return (1-r*r)*(2*s-1)*t*(t-1)/4.0;
        break;
        case 2:
          return (1-r*r)*s*(s-1)*(2*t-1)/4.0;
        break;
      }
    break;
    case 9:
      switch(m) {
        case 0:
          return (2*r-1)*(1-s*s)*t*(t-1)/4.0;
        break;
        case 1:
          return -r*(r-1)*s*t*(t-1)/2.0;
        break;
        case 2:
          return r*(r-1)*(1-s*s)*(2*t-1)/4.0;
        break;
      }
    break;
    case 10:
      switch(m) {
        case 0:
          return (2*r-1)*s*(s-1)*(1-t*t)/4.0;
        break;
        case 1:
          return r*(r-1)*(2*s-1)*(1-t*t)/4.0;
        break;
        case 2:
          return -r*(r-1)*s*(s-1)*t/2.0;
        break;
      }
    break;
    case 11:
      switch(m) {
        case 0:
          return (2*r+1)*(1-s*s)*t*(t-1)/4.0;
        break;
        case 1:
          return -r*(r+1)*s*t*(t-1)/2.0;
        break;
        case 2:
          return r*(r+1)*(1-s*s)*(2*t-1)/4.0;
        break;
      }
    break;
    case 12:
      switch(m) {
        case 0:
          return (2*r+1)*s*(s-1)*(1-t*t)/4.0;
        break;
        case 1:
          return r*(r+1)*(2*s-1)*(1-t*t)/4.0;
        break;
        case 2:
          return -r*(r+1)*s*(s-1)*t/2.0;
        break;
      }
    break;
    case 13:
      switch(m) {
        case 0:
          return -r*s*(s+1)*t*(t-1)/2.0;
        break;
        case 1:
          return (1-r*r)*(2*s+1)*t*(t-1)/4.0;
        break;
        case 2:
          return (1-r*r)*s*(s+1)*(2*t-1)/4.0;
        break;
      }
    break;
    case 14:
      switch(m) {
        case 0:
          return (2*r+1)*s*(s+1)*(1-t*t)/4.0;
        break;
        case 1:
          return r*(r+1)*(2*s+1)*(1-t*t)/4.0;
        break;
        case 2:
          return -r*(r+1)*s*(s+1)*t/2.0;
        break;
      }
    break;
    case 15:
      switch(m) {
        case 0:
          return (2*r-1)*s*(s+1)*(1-t*t)/4.0;
        break;
        case 1:
          return r*(r-1)*(2*s+1)*(1-t*t)/4.0;
        break;
        case 2:
          return -r*(r-1)*s*(s+1)*t/2.0;
        break;
      }
    break;
    case 16:
      switch(m) {
        case 0:
          return -r*s*(s-1)*t*(t+1)/2.0;
        break;
        case 1:
          return (1-r*r)*(2*s-1)*t*(t+1)/4.0;
        break;
        case 2:
          return (1-r*r)*s*(s-1)*(2*t+1)/4.0;
        break;
      }
    break;
    case 17:
      switch(m) {
        case 0:
          return (2*r-1)*(1-s*s)*t*(t+1)/4.0;
        break;
        case 1:
          return -r*(r-1)*s*t*(t+1)/2.0;
        break;
        case 2:
          return r*(r-1)*(1-s*s)*(2*t+1)/4.0;
        break;
      }
    break;
    case 18:
      switch(m) {
        case 0:
          return (2*r+1)*(1-s*s)*t*(t+1)/4.0;
        break;
        case 1:
          return -r*(r+1)*s*t*(t+1)/2.0;
        break;
        case 2:
          return r*(r+1)*(1-s*s)*(2*t+1)/4.0;
        break;
      }
    break;
    case 19:
      switch(m) {
        case 0:
          return -r*s*(s+1)*t*(t+1)/2.0;
        break;
        case 1:
          return (1-r*r)*(2*s+1)*t*(t+1)/4.0;
        break;
        case 2:
          return (1-r*r)*s*(s+1)*(2*t+1)/4.0;
        break;
      }
    break;
    case 20:
      switch(m) {
        case 0:
          return -r*(1-s*s)*t*(t-1);
        break;
        case 1:
          return -(1-r*r)*s*t*(t-1);
        break;
        case 2:
          return (1-r*r)*(1-s*s)*(2*t-1)/2.0;
        break;
      }
    break;
    case 21:
      switch(m) {
        case 0:
          return -r*s*(s-1)*(1-t*t);
        break;
        case 1:
          return (1-r*r)*(2*s-1)*(1-t*t)/2.0;
        break;
        case 2:
          return -(1-r*r)*s*(s-1)*t;
        break;
      }
    break;
    case 22:
      switch(m) {
        case 0:
          return (2*r-1)*(1-s*s)*(1-t*t)/2.0;
        break;
        case 1:
          return -r*(r-1)*s*(1-t*t);
        break;
        case 2:
          return -r*(r-1)*(1-s*s)*t;
        break;
      }
    break;
    case 23:
      switch(m) {
        case 0:
          return (2*r+1)*(1-s*s)*(1-t*t)/2.0;
        break;
        case 1:
          return -r*(r+1)*s*(1-t*t);
        break;
        case 2:
          return -r*(r+1)*(1-s*s)*t;
        break;
      }
    break;
    case 24:
      switch(m) {
        case 0:
          return -r*s*(s+1)*(1-t*t);
        break;
        case 1:
          return (1-r*r)*(2*s+1)*(1-t*t)/2.0;
        break;
        case 2:
          return -(1-r*r)*s*(s+1)*t;
        break;
      }
    break;
    case 25:
      switch(m) {
        case 0:
          return -r*(1-s*s)*t*(t+1);
        break;
        case 1:
          return -(1-r*r)*s*t*(t+1);
        break;
        case 2:
          return (1-r*r)*(1-s*s)*(2*t+1)/2.0;
        break;
      }
    break;
    case 26:
      switch(m) {
        case 0:
          return -2*r*(1-s*s)*(1-t*t);
        break;
        case 1:
          return -2*(1-r*r)*s*(1-t*t);
        break;
        case 2:
          return -2*(1-r*r)*(1-s*s)*t;
        break;
      }
    break;
  }

  return 0;


}

