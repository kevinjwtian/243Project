#define PB_EDGECAPTURE ((volatile long*) 0xFF20005C)
#define RLEDS ((volatile long*) 0xFF200000)
int main(){
	long PBreleases;
	int i;
	*PB_EDGECAPTURE = 0;
	int key;
	while(1){
		PBreleases = *PB_EDGECAPTURE;//variable to mess with
		key = PBreleases & 0b1111;
		if(PBreleases){
		for(i = 0; i<1000000;i++){
			//*RLEDS = PBreleases;
			switch (key)
			{
    		case 1: // code to be executed if key = 0;
					*RLEDS = 0x0001;
        		break;
    		case 2: // code to be executed if key = 1;
					*RLEDS = 0x0002;
        		break;
			case 4: // code to be executed if key = 2;
					*RLEDS = 0x0004;
        		break;
        	case 8: // code to be executed if key = 3;
					*RLEDS = 0x0008;
        		break;
    		default:
				break;
			}
		}
		*PB_EDGECAPTURE = PBreleases;
		}
		else{
			*RLEDS = 0;
		}		
	}
}