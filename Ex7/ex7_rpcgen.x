program RPC_PROG
{     
	version CURR_VER    
	{        
		int SMALLLETTER(string) = 1;        
		int SUMOFRATIONAL(double *) = 2;
		int SAMELETTER(string) = 3;                          
	} = 1; 
} = 17; 
