#define PB_EDGECAPTURE ((volatile long*) 0xFF20005C)
#define RLEDS ((volatile long*) 0xFF200000)
int main(){
	long PBreleases;
	int i;
	int key0; 
	int key1;
	int key2;
	*PB_EDGECAPTURE = 0;
	while(1){
		PBreleases = *PB_EDGECAPTURE;//variable to mess with
		
		if(PBreleases){
		for(i = 0; i<1000000;i++){
			*RLEDS = PBreleases;
		}
		*PB_EDGECAPTURE = PBreleases;
		}
		else{
			*RLEDS = 0;
		}
		
	}
}
	

