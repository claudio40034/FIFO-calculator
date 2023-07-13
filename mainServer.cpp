#include "Queue.h"
#include "Calculator.h"
#define FIFO_SERVER "/tmp/server"
#define FIFO_CLIENT "/tmp/client"
#define PERMS 0666

int* shm_ptr;
int shm_id;

int* nc; 
int* unlock;
int* next_PID; 
int* status;
int current_PID;

int readfd,writefd;



float read_fd_server(int,float);
void write_fd_server(int,float);
static void sig_hand(int);   
using namespace std;
int main(int argc,char* argv[]){


Queue g;
Calculator o;
int listen;
float write;
float rx1,rx2,x3;

  if (signal(SIGINT, sig_hand) == SIG_ERR) /* <CTRL+C> */
      perror("signal"), exit(1);


		 

  if ((shm_id = shmget((key_t)1124, 2*sizeof(int), 0666|IPC_CREAT)) == -1)
    perror("shmget"), exit(1);
  if ((shm_ptr = (int *)shmat(shm_id, (void*)0, 0)) == (void *)-1)
    perror("shmat failed"), exit(1);
g.gainMemory();

		 
nc = shm_ptr;
unlock = (shm_ptr+1);
next_PID = (shm_ptr+2);
status = (shm_ptr+3);

*unlock = 0;
*nc = 0;
*status = 1;

		
 if( (mknod(FIFO_SERVER,S_IFIFO|PERMS,0))< 0 && errno != EEXIST){
	cout<<"Can't create FIFO_SERVER"<<endl;
	exit(1);
	}

 if( (mknod(FIFO_CLIENT,S_IFIFO|PERMS,0))< 0 && errno != EEXIST){
	cout<<"Can't create FIFO_CLIENT"<<endl;
	exit(1);
	}
		 
	  while(*status == 1){ 
cout<<"The server is waiting for a client to connect..."<<endl;
 if( (readfd = open(FIFO_CLIENT,0)) < 0){
	cout<<"Sever: Can't open a comunication with client"<<endl;
	exit(3);
	}
 if( (read(readfd,&listen,sizeof(listen))) < 0) {
	cout<<"Server: Can't read client's PID"<<endl;
	exit(3);
	}

*unlock = -1;
close(readfd);
current_PID = listen;			
	cout<<endl<<"Connection with client opened "<<current_PID<<endl;
listen = 5;

	  while(listen !=0){

	cout<<"I'm waiting for a request from the client..."<<endl;
	
 if( (readfd = open(FIFO_CLIENT,0)) < 0){
	cout<<"Server: Can't open a comunication with client "<<current_PID<<endl;
	exit(3);
	}
 if( (read(readfd,&listen,sizeof(listen))) < 0){
	cout<<"Server: Can't get the request made by the client "<<current_PID<<endl;
	exit(3);
 }

		 
		switch(listen){
   case 0:
	cout<<"Client "<<current_PID<<" ends communicatione..."<<endl;
   	cout<<endl;
   	*unlock = 0;
   	(*nc)--;
 if( ((*next_PID) = g.dequeue()) == -1){
	cout<<"There is no client in the queue..."<<endl;
 }else{	
	cout<<"The next PID to be executed is: "<<*next_PID<<endl;
 }
current_PID = 0;
  
   break;
   case 1:
	cout<<"The client "<<current_PID<<" requested the product operation"<<endl;
rx1 = read_fd_server(current_PID,rx1);
rx2 = read_fd_server(current_PID,rx2);
write = o.getMultiplication(rx1,rx2);
write_fd_server(current_PID,write);
   
   break;
   case 2:
	cout<<"The client "<<current_PID<<" requested the subtraction"<<endl;
rx1 = read_fd_server(current_PID,rx1);
rx2 = read_fd_server(current_PID,rx2);
write = o.getSubtraction(rx1,rx2);
write_fd_server(current_PID,write);
   
   break;
   case 3:
	cout<<"Client "<<current_PID<<" requested the division"<<endl;
rx1 = read_fd_server(current_PID,rx1);
rx2 = read_fd_server(current_PID,rx2);
write = o.getDivision(rx1,rx2);
write_fd_server(current_PID,write);
   
   break;
   case 4:
	cout<<"Client "<<current_PID<<" requested the sum"<<endl;
rx1 = read_fd_server(current_PID,rx1);
rx2 = read_fd_server(current_PID,rx2);
write = o.getSum(rx1,rx2);
write_fd_server(current_PID,write);
   
   break;
		}
		  }
		  } 


return 0;
}

void write_fd_server(int a,float b){
  if( (writefd = open(FIFO_SERVER,1)) < 0) {
	cout<<"Server: "<<"Can't open the comunication with client "<<a<<endl;
	exit(2);
  }
  if( (write(writefd,&b,sizeof(b))) < 0) {
	cout<<"Server: "<<"Can't send the result to the client "<<a<<endl;
	exit(2);
  }
  close(writefd);
}

float read_fd_server(int a,float b){
  if( (readfd = open(FIFO_CLIENT,0)) < 0) {
	cout<<"Server: "<<"Can't open the comunication with client  "<<a<<endl;
	exit(3);
  }
  if( (read(readfd,&b,sizeof(b))) < 0) {
	cout<<"Server: "<<"Can't read the operands of the client  "<<a<<endl;
	exit(3);
  }
  close(readfd);
return b;
}

static void sig_hand(int signo){  
  if(signo == SIGINT){
cout<<endl<<"Terminating..."<<endl;
  if(current_PID!=0)
kill((pid_t)current_PID,SIGTERM);
*status = 0;
sleep(10);
shmdt(shm_ptr);
shmctl(shm_id, IPC_RMID, 0);	
kill(getpid(),SIGTERM); 
  }  
}
