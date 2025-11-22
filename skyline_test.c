#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ftSkyline.h"
// I cannot tell why this one cannot link with the math library...

int main() {
	struct futhark_context_config *cfg = futhark_context_config_new();
    struct futhark_context *ctx = futhark_context_new(cfg);
    printf("Set up futhark core.\n");

    // Define SkylineBase
      float mins[3] = {0,0,0};
      float maxs[3] = {15,15,15};
      int64_t grid_per_dim[3] = {1,1,1};
      int64_t angl_per_dim[2] = {6,6};

      struct futhark_opaque_skylineBase_float *skB;
	  struct futhark_f32_1d *mins_ft = futhark_new_f32_1d(ctx,mins,3);
	  struct futhark_f32_1d *maxs_ft = futhark_new_f32_1d(ctx,maxs,3);
	  struct futhark_i64_1d *grids_ft = futhark_new_i64_1d(ctx, grid_per_dim, 3);
	  struct futhark_i64_1d *angls_ft = futhark_new_i64_1d(ctx, angl_per_dim, 2);

	  futhark_entry_define_skyline_space_float(ctx, &skB, mins_ft, maxs_ft, grids_ft, angls_ft);
	  futhark_context_sync(ctx);

	  futhark_free_f32_1d(ctx, mins_ft);
	  futhark_free_f32_1d(ctx, maxs_ft);
	  futhark_free_i64_1d(ctx, grids_ft);
	  futhark_free_i64_1d(ctx, angls_ft);

	  printf("Set up SkylineBase - checking that it's been set properly...\n");
	  int64_t grid_no, angl_no, part_no;
	  futhark_project_opaque_skylineBase_float_total_grid_no(ctx, &grid_no, skB);
	  futhark_project_opaque_skylineBase_float_total_angle_no(ctx, &angl_no, skB);
	  futhark_project_opaque_skylineBase_float_total_part_no(ctx, &part_no, skB);
	  printf("SkB:\n\tGrid partitions: %ld\n\tAngle partitions: %ld\n\tTotal partitions: %ld\n\n", grid_no, angl_no, part_no);

	float points[40*3] = 
		{
			6,1,1, 6.4,0.5,0.2, 5.6,1.4,0.3, 6.5,2.3,0.4,
			1,6,4, 0.6,6.6,0.4, 1.2,5.8,2.2, 2.2,6.2,3, 
			6,12,1, 5,13,1, 6.2,11,1, 5.1,12,1, 
			2,2,6.2, 1,3,6, 1,2,6.1, 2,1,6.1, 
			7,9.5,8, 9,5.5,6, 5.1,5.7,5.2, 8,5,6, 
			12,9,9.5, 15,6,8, 12,6,8, 10,8,6, 
			12,14,9.4, 15,11,7.9, 12,11,7.9, 10,13,6.1, 
			10,4,10, 11,3,11, 10,2,11, 11,1,10, 
			5,5,11, 6,5,10, 6,5.2,12, 7,5.3,12, 
			6,12,12, 7,11,13, 5,11,12, 6,12,10

		};
	printf("Initialised points.\n");

	//struct futhark_f32_2d *skyData_ft = futhark_new_f32_2d(ctx, points, 40, 3);

}