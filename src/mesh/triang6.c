/*------------ -------------- -------- --- ----- ---   --       -            -
 *  wasora's mesh-related second-order triangle element routines
 *
 *  Copyright (C) 2017--2018 jeremy theler
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

int mesh_triang6_init(void) {

  double r[2];
  element_type_t *element_type;
  int j;
  
  element_type = &wasora_mesh.element_type[ELEMENT_TYPE_TRIANGLE6];
  element_type->name = strdup("triang6");
  element_type->id = ELEMENT_TYPE_TRIANGLE6;
  element_type->dim = 2;
  element_type->order = 2;
  element_type->nodes = 6;
  element_type->faces = 3;
  element_type->nodes_per_face = 3;
  element_type->h = mesh_triang6_h;
  element_type->dhdr = mesh_triang6_dhdr;
  element_type->point_in_element = mesh_point_in_triangle;
  element_type->element_volume = mesh_triang_vol;

  // coordenadas de los nodos
/*
Triangle6:    
    
2             
|`\           
|  `\         
5    `4       
|      `\     
|        `\   
0-----3----1  
*/     
  element_type->node_coords = calloc(element_type->nodes, sizeof(double *));
  element_type->node_parents = calloc(element_type->nodes, sizeof(node_relative_t *));
  for (j = 0; j < element_type->nodes; j++) {
    element_type->node_coords[j] = calloc(element_type->dim, sizeof(double));  
  }
  
  element_type->first_order_nodes++;
  element_type->node_coords[0][0] = 0;
  element_type->node_coords[0][1] = 0;
  
  element_type->first_order_nodes++;
  element_type->node_coords[1][0] = 1;  
  element_type->node_coords[1][1] = 0;
  
  element_type->first_order_nodes++;
  element_type->node_coords[2][0] = 0;  
  element_type->node_coords[2][1] = 1;

  wasora_mesh_add_node_parent(&element_type->node_parents[3], 0);
  wasora_mesh_add_node_parent(&element_type->node_parents[3], 1);
  wasora_mesh_compute_coords_from_parent(element_type, 3);
 
  wasora_mesh_add_node_parent(&element_type->node_parents[4], 1);
  wasora_mesh_add_node_parent(&element_type->node_parents[4], 2);
  wasora_mesh_compute_coords_from_parent(element_type, 4);
  
  wasora_mesh_add_node_parent(&element_type->node_parents[5], 2);
  wasora_mesh_add_node_parent(&element_type->node_parents[5], 0);
  wasora_mesh_compute_coords_from_parent(element_type, 5); 
  
  // gauss points and extrapolation matrices
  element_type->gauss = calloc(2, sizeof(gauss_t));
  
  // full integration: 3 points
  mesh_gauss_init_triang3(element_type, &element_type->gauss[integration_full]);
  element_type->gauss[integration_full].extrap = gsl_matrix_calloc(element_type->nodes, 3);

  // reduced integration: 1 point
  mesh_gauss_init_quad1(element_type, &element_type->gauss[integration_reduced]);
  element_type->gauss[integration_reduced].extrap = gsl_matrix_calloc(element_type->nodes, 1);
  
  // the two extrapolation matrices
  // first the full one
  r[0] = -1.0/3.0;
  r[1] = -1.0/3.0;
  for (j = 0; j < 3; j++) {
    gsl_matrix_set(element_type->gauss[integration_full].extrap, 0, j, mesh_triang3_h(j, r));
  }  
  
  r[0] = +5.0/3.0;
  r[1] = -1.0/3.0;
  for (j = 0; j < 3; j++) {
    gsl_matrix_set(element_type->gauss[integration_full].extrap, 1, j, mesh_triang3_h(j, r));
  }  

  r[0] = -1.0/3.0;
  r[1] = +5.0/3.0;
  for (j = 0; j < 3; j++) {
    gsl_matrix_set(element_type->gauss[integration_full].extrap, 2, j, mesh_triang3_h(j, r));
  }  

  r[0] = +2.0/3.0;
  r[1] = -1.0/3.0;
  for (j = 0; j < 3; j++) {
    gsl_matrix_set(element_type->gauss[integration_full].extrap, 3, j, mesh_triang3_h(j, r));
  }  

  r[0] = +2.0/3.0;
  r[1] = +2.0/3.0;
  for (j = 0; j < 3; j++) {
    gsl_matrix_set(element_type->gauss[integration_full].extrap, 4, j, mesh_triang3_h(j, r));
  }  

  r[0] = -1.0/3.0;
  r[1] = +2.0/3.0;
  for (j = 0; j < 3; j++) {
    gsl_matrix_set(element_type->gauss[integration_full].extrap, 5, j, mesh_triang3_h(j, r));
  }  
  
  // the reduced one is a vector of ones
  for (j = 0; j < element_type->nodes; j++) {
    // reduced
    gsl_matrix_set(element_type->gauss[integration_reduced].extrap, j, 0, 1.0);
  }
  
  return WASORA_RUNTIME_OK;    
}

double mesh_triang6_h(int j, double *vec_r) {
  double r = vec_r[0];
  double s = vec_r[1];

  switch (j) {
    case 0:
      return (1-r-s)*(2*(1-r-s)-1);
      break;
    case 1:
      return r*(2*r-1);
      break;
    case 2:
      return s*(2*s-1);
      break;
      
    case 3:
      return 4*(1-r-s)*r;
      break;
    case 4:
      return 4*r*s;
      break;
    case 5:
      return 4*s*(1-r-s);
      break;
  }

  return 0;

}

double mesh_triang6_dhdr(int j, int m, double *vec_r) {
  double r = vec_r[0];
  double s = vec_r[1];

  switch(j) {
    case 0:
      if (m == 0) {
        return 1-4*(1-r-s);
      } else {
        return 1-4*(1-r-s);
      }
      break;
    case 1:
      if (m == 0) {
        return 4*r-1;
      } else {
        return 0;
      }
      break;
    case 2:
      if (m == 0) {
        return 0;
      } else {
        return 4*s-1;
      }
      break;
      
    case 3:
      if (m == 0) {
        return 4*(1-r-s)-4*r;
      } else {
        return -4*r;
      }
      break;
    case 4:
      if (m == 0) {
        return 4*s;
      } else {
        return 4*r;
      }
      break;
    case 5:
      if (m == 0) {
        return -4*s;
      } else {
        return 4*(1-r-s)-4*s;
      }
      break;

  }

  return 0;


}
