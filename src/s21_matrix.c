#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int res = OK;
  if (rows > 0 && columns > 0) {
    result->rows = rows;
    result->columns = columns;
    result->matrix = malloc(result->rows * sizeof(double *));

    if (result->matrix) {
      for (int i = 0; i < result->rows; i++) {
        result->matrix[i] = malloc(result->columns * sizeof(double));
        if (!result->matrix[0]) {
          res = ALLOCATION_ERROR;
          for (int j = 0; j < i; j++) {
            free(result->matrix[j]);
          }
          free(result->matrix);
        }
      }
    } else {
      res = ALLOCATION_ERROR;
    }
  } else {
    res = INCORRECT_MATRIX;
  }
  return res;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
      A->matrix[i] = NULL;
    }
    free(A->matrix);
    A->matrix = NULL;
    A->columns = 0;
    A->rows = 0;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int res = SUCCESS;
  if ((A->columns == B->columns) && (A->rows == B->rows)) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if ((fabs(A->matrix[i][j] - B->matrix[i][j])) > S21_EPS) {
          res = FAILURE;
        }
      }
    }
  } else {
    res = FAILURE;
  }
  return res;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;

  if (A->matrix == NULL || B->matrix == NULL || A->rows == 0 ||
      A->columns == 0 || B->rows == 0 || B->columns == 0) {
    res = INCORRECT_MATRIX;
  }
  if (!((A->rows == B->rows) && (A->columns == B->columns)) && !res) {
    res = CALCULATION_ERROR;
  }

  if (res == OK) {
    res = s21_create_matrix(A->rows, A->columns, result);
  }

  if (res == OK) {
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }

  return res;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;

  if (A->matrix == NULL || B->matrix == NULL || A->rows == 0 ||
      A->columns == 0 || B->rows == 0 || B->columns == 0) {
    res = INCORRECT_MATRIX;
  }
  if (!((A->rows == B->rows) && (A->columns == B->columns)) && !res) {
    res = CALCULATION_ERROR;
  }

  if (res == OK) {
    res = s21_create_matrix(A->rows, A->columns, result);
  }

  if (res == OK) {
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }

  return res;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int res = OK;

  if (A->matrix == NULL || A->rows == 0 || A->columns == 0) {
    res = INCORRECT_MATRIX;
  }
  if (res == OK) {
    res = s21_create_matrix(A->rows, A->columns, result);
  }

  if (res == OK) {
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }

  return res;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;

  if (A->matrix == NULL || B->matrix == NULL || A->rows == 0 ||
      A->columns == 0 || B->rows == 0 || B->columns == 0) {
    res = INCORRECT_MATRIX;
  }

  if (!((A->columns == B->rows) && (A->rows == B->columns)) && !res) {
    res = CALCULATION_ERROR;
  }

  if (res == OK) {
    res = s21_create_matrix(A->rows, B->columns, result);
  }

  if (res == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        result->matrix[i][j] = 0.0;
        for (int k = 0; k < A->columns; k++) {
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
  }

  return res;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int res = OK;

  if (A->matrix == NULL || A->rows == 0 || A->columns == 0) {
    res = INCORRECT_MATRIX;
  }

  if (res == OK) {
    res = s21_create_matrix(A->columns, A->rows, result);
  }

  if (res == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }

  return res;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int res = OK;
  int size = A->columns;
  printf("%d\n",size);

  if (A->rows != A->columns) {
    res = CALCULATION_ERROR;
     printf("CALCULATION_ERROR\n");
  }

  if (A->matrix == NULL || A->rows == 0 || A->columns == 0) {
    res = INCORRECT_MATRIX;
    printf("INCORRECT_MATRIX\n");
  }

  if (res == OK) {
    res = s21_create_matrix(A->rows, A->columns, result);
    printf("s21_create_matrix\n");
  }

  if (res == OK) {
    printf("OK\n");
    double minor = 0;
    matrix_t matrix_tmp;
    if (size != 1) {
      printf("MAIN\n");
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          s21_create_matrix(size - 1, size - 1, &matrix_tmp);
          matrix_without_row_col(A, &matrix_tmp, i, j);
          s21_determinant(&matrix_tmp, &minor);
          s21_remove_matrix(&matrix_tmp);

          result->matrix[j][i] = minor * pow(-1, i + j);
        }
      }
    } else {
      result->matrix[0][0] = 1;
      printf("OK");
    }
  }
  return res;
}

int s21_determinant(matrix_t *A, double *result) {
  int res = OK;
  *result = 0;
  if (A->rows != A->columns) {
    res = CALCULATION_ERROR;
  }

  if (A->matrix == NULL || A->rows == 0 || A->columns == 0) {
    res = INCORRECT_MATRIX;
  }

  if (res == OK) {
    int size = A->columns;
    if (size == 1) {
      *result = A->matrix[0][0];
    }

    if (size == 2) {
      *result =
          A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    }

    if (size > 2) {
      for (int i = 0; i < size; i++) {
        double minor = 0;
        matrix_t matrix_minor;

        s21_create_matrix(size - 1, size - 1, &matrix_minor);
        matrix_without_row_col(A, &matrix_minor, i, 0);
        s21_determinant(&matrix_minor, &minor);

        *result = minor * pow(-1, i) * A->matrix[0][i] + *result;

        s21_remove_matrix(&matrix_minor);
      }
    }
  }
  return res;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int res = OK;

  if (A->matrix == NULL || A->rows == 0 || A->columns == 0) {
    res = INCORRECT_MATRIX;
  }

  if (A->rows != A->columns) {
    res = CALCULATION_ERROR;
  }

  if (res == OK) {
    double det = 0.0;
    res = s21_determinant(A, &det);
    if (res == OK) {
      if (det == 0) {
        res = CALCULATION_ERROR;
      } else {
        if (A->rows == 1 && A->columns == 1) {
          result->matrix[0][0] = 1.0 / A->matrix[0][0];
        } else {
          matrix_t complements = {0};
          res = s21_calc_complements(A, &complements);
          if (res == OK) {
            matrix_t trans = {0};
            res = s21_transpose(&complements, &trans);
            if (res == OK) {
              res = s21_mult_number(&trans, 1.0 / det, result);
              s21_remove_matrix(&trans);
            }
            s21_remove_matrix(&complements);
          }
        }
      }
    }
  }
  return res;
}

void matrix_without_row_col(matrix_t *A, matrix_t *result, int col, int row) {
  int row_skip = 0;
  int col_skip = 0;
  for (int i = 0; i < A->rows - 1; i++) {
    if (i == row) {
      row_skip = 1;
    }
    for (int j = 0; j < A->columns - 1; j++) {
      if (j == col) {
        col_skip = 1;
      }
      result->matrix[i][j] = A->matrix[i + row_skip][j + col_skip];
    }
    col_skip = 0;
  }
}

void matrix_fill(matrix_t *A, double result[A->rows][A->columns]) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      A->matrix[i][j] = result[i][j];
    }
  }
}