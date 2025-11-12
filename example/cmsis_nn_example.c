#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "arm_nnfunctions.h"
#include "arm_nnsupportfunctions.h"
#include "../code/gcov_public.h"
#ifdef USING_FVP_CORSTONE_300
extern void uart_init(void);
#endif

void setUp(void)
{ 
#ifdef USING_FVP_CORSTONE_300
    uart_init();
#endif
}

void tearDown(void) {}


// Simple ReLU activation function (CMSIS-NN style)
void arm_relu_q7(int8_t *data, uint16_t size)
{
  uint16_t i;
  
  for (i = 0; i < size; i++)
    {
      if (data[i] < 0)
        data[i] = 0;
    }
}

int
main (void)
{
  setUp();
  int i;
  int8_t test_data[35] = {5, 3, -2, 8, 0, -1, 6, 2, -3, 4,
                          7, -4, 5, 1, -6, 9, 3, -1, 2, 6, 33, -3, 1, 2, 3, 4, 5, 6, 7, 5,-11, -12, -13,56,-100};
  int8_t pooled_output[21];  // Output after 2x2 max pooling with stride 2
  
  // Because we are running this example under unix,
  // do not need __gcov_call_constructors() here,
  // unix application startup will accomplish that.
  
  printf("Original data (4x5 tensor):\n");
  for (i = 0; i < 20; i++)
    {
      printf("%3d ", test_data[i]);
      if ((i + 1) % 5 == 0)
        printf("\n");
    }
  
  // Apply ReLU activation (zeros out negative values)
  arm_relu_q7(test_data, 20);
  
  printf("\nAfter ReLU:\n");
  for (i = 0; i < 20; i++)
    {
      printf("%3d ", test_data[i]);
      if ((i + 1) % 5 == 0)
        printf("\n");
    }
  
  // Set up CMSIS-NN max pooling parameters
  cmsis_nn_context ctx = {NULL, 0};
  
  cmsis_nn_pool_params pool_params;
  pool_params.stride.h = 1;
  pool_params.stride.w = 2;
  pool_params.padding.h = 0;
  pool_params.padding.w = 0;
  pool_params.activation.min = -128;
  pool_params.activation.max = 127;
  
  cmsis_nn_dims input_dims;
  input_dims.n = 1;  // batch size
  input_dims.h = 1;  // height
  input_dims.w = 5;  // width
  input_dims.c = 7;  // channels
  
  cmsis_nn_dims filter_dims;
  filter_dims.h = 1;  // filter height
  filter_dims.w = 2;  // filter width
  
  cmsis_nn_dims output_dims;
  output_dims.n = 1;
  output_dims.h = 1;  // 1
  output_dims.w = 3;  // (5 - 2) / 2 + 1 = 2
  output_dims.c = 7;
  
  // Perform CMSIS-NN max pooling
  arm_cmsis_nn_status status = arm_max_pool_s8(&ctx,
                                               &pool_params,
                                               &input_dims,
                                               test_data,
                                               &filter_dims,
                                               &output_dims,
                                               pooled_output);
  
  if (status == ARM_CMSIS_NN_SUCCESS)
    {
      printf("\nAfter 2x2 Max Pooling (stride=2):\n");
      for (i = 0; i < output_dims.h * output_dims.w * output_dims.c; i++)
        {
          printf("%3d ", pooled_output[i]);
          if ((i + 1) % output_dims.w == 0)
            printf("\n");
        }
    }
  else
    {
      printf("\nMax pooling failed with status: %d\n", status);
    }
  
  // Because we are running this example under unix,
  // the unix application shutdown will call
  // __gcov_exit() once per source file,
  // so several copies of the gcov output
  // will be produced.
  // In most embedded systems, that would not happen,
  // instead you would insert your own single
  // call to __gcov_exit() here,
  // to produce one copy of the gcov output.
  tearDown();
  (void)fflush(stdout);
  // return 0;
}

