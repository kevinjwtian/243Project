#define RLEDS ((volatile long*) 0xFF200000)

int main(){
	unsigned char byte1 = 0;
	unsigned char byte2 = 0;
	unsigned char byte3 = 0;	

	volatile int * PS2_ptr = (int *)0xFF200100; //PS2 address

	int PS2_data, RVALID;

	while(1){
			PS2_data = *PS2_ptr; //Read from ps2
			RVALID = (PS2_data & 0x8000);
			if(RVALID !=0){
				
				//save the last three bytes read
				byte1 = byte2;
				byte2 = byte3;
				byte3 = PS2_data & 0xFF;
				
			}
			
			/*if((byte2 == 0xAA)&&(byte3 ==0x00)){
				*(PS2_ptr) = 0xF4;
			}*/
			//Check led
			if((byte1 == 0x1C)&&(byte2 == 0xF0)&&(byte3 == 0x1C)){//should be A pressed
				for(int i = 0; i<250000;i++)
			 *RLEDS = 0x380;
				byte1 = 0;
				byte2 = 0;
				byte3 = 0;
			}
			else
				*RLEDS = 0x0;
		//make into case
			if((byte1 == 0x23)&&(byte2 == 0xF0)&&(byte3 == 0x23)){//should be A pressed
				for(int i = 0; i<250000;i++)
			 *RLEDS = 7;
				byte1 = 0;
				byte2 = 0;
				byte3 = 0;
			}
			else
				*RLEDS = 0x0;

	}
}