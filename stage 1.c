// global variable
volatile int pixel_buffer_start; 

//Data structures
struct point {
    int x;
    int y;
    int dx;
    int dy;
};

//function protypes
void clear_screen ();
void draw_line (int x0, int y0, int x1, int y1, short int color);
void wait_for_vsync();
int rgb_to_pixel_color (int r, int g, int b);
void swap (int* x, int* y);
void plot_pixel(int x, int y, short int line_color);
void randomize_all_points(struct point points[]);
void draw_connecting_lines (struct point points[]);
void update_all_points(struct point points[]);
void draw_box(struct point);
bool is_in_range (int x, int lower, int upper);
void draw_all_boxes(struct point points[]);
int random_in_range(int lower, int upper);
//constant definitions
#define NUM_POINTS 8
#define PB_EDGECAPTURE ((volatile long*) 0xFF20005C)
#define RLEDS ((volatile long*) 0xFF200000)


// main program
int main(void) {
    //VGA
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables
    struct point points[NUM_POINTS];
    // initialize location and direction of rectangles
    randomize_all_points(points);

    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
	
	//declare input variables
	volatile int lane = 230;// x-cord of lane
	long PBreleases;
	int i;
	*PB_EDGECAPTURE = 0;
	int key;
	
	//declare car
	struct point car_center;
	car_center.x = lane;
	car_center.y = 180; 
	car_center.dy = 0;
	car_center.dx =0; //use later
		
    while (1) { //in game loop
		//Check interupt
		PBreleases = *PB_EDGECAPTURE;//variable to mess with
		key = PBreleases & 0b1111;
		if(PBreleases){
			//*RLEDS = PBreleases;
			switch (key)
			{
    		case 1: // code to be executed if key = 0;
					if(lane<289){
					lane = lane + 60;
					car_center.x = lane;
					
					for(i = 0; i<250000;i++){
					*RLEDS = 0x7;
					}
					for(i = 0; i<250000;i++){
					*RLEDS = 0x0;
					}
					for(i = 0; i<250000;i++){
					*RLEDS = 0x7;
					}
					for(i = 0; i<250000;i++){
					*RLEDS = 0x0;
					}
					}
        		break;
    		case 2: // code to be executed if key = 1;
					if(lane>229){
					lane = lane - 60;
					car_center.x = lane;
					
					for(i = 0; i<250000;i++){
					*RLEDS = 0x380;
					}
					for(i = 0; i<250000;i++){
					*RLEDS = 0x0;
					}
					for(i = 0; i<250000;i++){
					*RLEDS = 0x380;
					}
					for(i = 0; i<250000;i++){
					*RLEDS = 0x0;
					}
						
					}
        		break;
			case 4: // code to be executed if key = 2;
					for(i = 0; i<1000000;i++)
					*RLEDS = 0x0004;
        		break;
        	case 8: // code to be executed if key = 3;
					for(i = 0; i<1000000;i++)
					*RLEDS = 0x0008;
        		break;
    		default:
				break;
			}
		
		*PB_EDGECAPTURE = PBreleases;
		}
		else{
			*RLEDS = 0;
		}
        /* Erase any boxes and lines that were drawn in the last iteration */
        memset((short int*) pixel_buffer_start,0,245760); //better
        /* Or draw blocks again in black instead*/
        // Draw LANES
        draw_line(260, 0, 260, 40, 0xFFFF);   // first
        draw_line(260, 60, 260, 100, 0xFFFF);  
        draw_line(260, 120, 260, 160, 0xFFFF);   
        draw_line(260, 180, 260, 220, 0xFFFF);     

        draw_line(200, 0, 200, 40, 0xFFFF);   // second
        draw_line(200, 60, 200, 100, 0xFFFF);  
        draw_line(200, 120, 200, 160, 0xFFFF);   
        draw_line(200, 180, 200, 220, 0xFFFF); 

        draw_line(140, 0, 140, 239, 0xFF00); // no lane change 
        draw_line(130, 0, 130, 239, 0xFF00); 
        // end draw lines

        /* game objects*/
		draw_car(car_center);
        /*end game objects*/
        wait_for_vsync(); 
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); 

    }//end of game loop

}//end of main

void draw_all_boxes(struct point points[]) {
    for (int i = 0; i < NUM_POINTS; i++) {
        draw_box(points[i]);
    }
}
void draw_box (struct point p) {// p is centre
    int box_size = 2;
    for (int x = p.x-box_size; x <= p.x+box_size; x++) {
        for (int y = p.y-box_size; y <= p.y+box_size; y++) {
            if(is_in_range(x, 0, 319) && is_in_range(y, 0, 239)) {
                plot_pixel(x, y, 0xFFFF);
            }
        }
    }
}

void draw_car (struct point p) {
	int box_size = 10;
    for (int x = p.x-box_size; x <= p.x+box_size; x++) {
        for (int y = p.y-box_size; y <= p.y+box_size; y++) {
            if(is_in_range(x, 0, 319) && is_in_range(y, 0, 239)) {
                plot_pixel(x, y, 0xFFFF);
            }
        }
    }
	
}

bool is_in_range (int x, int lower, int upper) {
    return (x >= lower && x <= upper);
}
void draw_connecting_lines (struct point points[]) {
    for (int i = 0; i < NUM_POINTS-1; i++) {
        draw_line (points[i].x, points[i].y, points[i+1].x, points[i+1].y, 0xFFFF);
    }

    draw_line (points[0].x, points[0].y, points[NUM_POINTS-1].x, points[NUM_POINTS-1].y, 0xFFFF);
}
void update_all_points(struct point points[]) {
    for (int i = 0; i < NUM_POINTS; i++) {
        if (points[i].x == 0 || points[i].x == 319) points[i].dx = -points[i].dx;
        if (points[i].y == 0 || points[i].y == 239) points[i].dy = -points[i].dy;
        points[i].x += points[i].dx;
        points[i].y += points[i].dy;
    }
}
void randomize_all_points (struct point points[]){
    for (int i = 0; i < NUM_POINTS; i++) {
        points[i].x = random_in_range(0, 319);
        points[i].y = random_in_range(0, 239);
        points[i].dx = rand() % 2 * 2 - 1;
        points[i].dy = rand() % 2 * 2 - 1;
    }
}

int random_in_range (int lower, int upper) {
    int random_num = (rand() % (upper-lower + 1)) + lower;
    return random_num;
}
void wait_for_vsync(){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020; //buffer register pixel controller
    register int status;
    *pixel_ctrl_ptr = 1; //start the synchronization process
    //wait for S to become 0
    status=*(pixel_ctrl_ptr +3);
    while((status & 0x01) != 0) {   
    status = *(pixel_ctrl_ptr +3); 
    }
}

void clear_screen() {//not used currently
    int x;
    int y;
    for (x = 0; x < 320; x++)
        for (y = 0; y < 240; y++)
            plot_pixel(x, y, 0x0000);   
}

void draw_line (int x0, int y0, int x1, int y1, short int color) {
    bool is_steep = abs(y1 - y0) > abs (x1 -x0);
    if (is_steep) {
        swap(&x0, &y0);
        swap(&x1, &y1);
    }
    if (x0 > x1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    int deltax = x1 - x0;
    int deltay = abs(y1 - y0);
    int error = -(deltax / 2);
    int y = y0;
    int y_step;
    if (y0 < y1) {
        y_step = 1;
    } else {
        y_step = -1;
    }

    for (int x = x0; x <= x1; x++) {
        if (is_steep) {
            plot_pixel(y, x, color);
        } else {
            plot_pixel(x, y, color);
        }
        error += deltay;
        if (error >= 0) {
            y += y_step;
            error -= deltax;
        }
    }
}

void plot_pixel(int x, int y, short int line_color){
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}


void swap (int* x, int* y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

