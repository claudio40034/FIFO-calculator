#include "Queue.h" 
#define FIFO_SERVER "/tmp/server"
#define FIFO_CLIENT "/tmp/client"
#define PERMS 0666
#define MAX_CLIENTS 4

int* shm_ptr;
int shm_id;

int* nc; 
int* unlock;
int* ready;
int* status_server;

int readfd,writefd;

void write_fd(int,int);
void write_fd_client_operation(int ,float);
float read_fd_output(int,float);
using namespace std;

int main(int argc,char* argv[]){
float x1,x2,x3;
int select;
Queue c;
  
 		
 if ((shm_id = shmget((key_t)1124, 2*sizeof(int), 0666)) == -1)
    perror("shmget"), exit(1);
 if ((shm_ptr = (int *)shmat(shm_id, (void*)0, 0)) == (void *)-1)
    perror("shmat failed"), exit(1); 

c.gainMemory();
nc = shm_ptr; 
unlock = (shm_ptr+1); 
ready = (shm_ptr+2);
status_server = (shm_ptr+3);
(*nc)++; 
		
  if((*nc) > MAX_CLIENTS){
	cout<<"The maximum number of clients has been reached, you are ending..."<<endl;
kill(getpid(),SIGTERM);
  }


int own_PID = getpid();
	cout<<"I'm the pid "<<own_PID<<endl;
		
  if((*nc)>1){
c.enqueue(own_PID);
	cout<<"Put in the queue"<<endl;
  }
		
	 while( (*unlock)!= 0){
  
cout<<"Server is busy, check every 10 seconds..."<<endl;
sleep(10);
	 while((*ready)!=own_PID){
sleep(10);
  if( (*status_server) == 0) kill(own_PID,SIGTERM);
	 }
	 }

  if( (mknod(FIFO_CLIENT,S_IFIFO|PERMS,0))< 0 && errno != EEXIST){
	cout<<"Can't create FIFO_CLIENT"<<endl;
	exit(1);
  }
write_fd(own_PID,own_PID);

select = 1; 


	 while(select != 0){
	cout<<endl<<"0-Terminate the execution"<<endl<<"1-Multiplication"<<endl<<"2-Subtraction"<<endl<<"3-Division"<<endl<<"4-Sum"<<endl<<"5-Show queue"<<endl;
	cout<<endl<<"Make your choice: ";
	cin>>select; 
		switch(select){
   case 0: 
	cout<<"The connection to the server will drop..."<<endl;
write_fd(own_PID,select);
kill(own_PID,SIGTERM);
   break;
   case 1: 
write_fd(own_PID,select);
	cout<<"Insert operand 1: ";
	cin>>x1;  
write_fd_client_operation(own_PID,x1);
	cout<<"Insert operand 2: ";
	cin>>x2; 
write_fd_client_operation(own_PID,x2);
x3 = read_fd_output(own_PID,x3);
	cout<<"Multiplication's output is: "<<fixed<<setprecision(2)<<x3<<endl;
   break;
   case 2: 
write_fd(own_PID,select);
	cout<<"Insert operand 1: ";
	cin>>x1;  
write_fd_client_operation(own_PID,x1);
	cout<<"Insert operand 2: ";
	cin>>x2; 
write_fd_client_operation(own_PID,x2);
x3 = read_fd_output(own_PID,x3);
	cout<<"Subtraction's output is: "<<fixed<<setprecision(2)<<x3<<endl;
   break;
      case 3: 
write_fd(own_PID,select);
	cout<<"Insert operand 1: ";
	cin>>x1;  
write_fd_client_operation(own_PID,x1);
	cout<<"Insert operand 2: ";
	cin>>x2; 
write_fd_client_operation(own_PID,x2);
x3 = read_fd_output(own_PID,x3);
	cout<<"Division's output is: "<<fixed<<setprecision(2)<<x3<<endl;
   break;
   case 4:
write_fd(own_PID,select);
	cout<<"Insert operand 1: ";
	cin>>x1;  
write_fd_client_operation(own_PID,x1);
	cout<<"Insert operand 2: ";
	cin>>x2; 
write_fd_client_operation(own_PID,x2);
x3 = read_fd_output(own_PID,x3);
	cout<<"Sum's output is: "<<fixed<<setprecision(2)<<x3<<endl;
   break;
   case 5: 
c.printQueue();
   break;
   
   
		}
	 }

return 0;
}

		/* comunico al server il mio pid con questa funzione */
void write_fd(int a,int b){
  if( (writefd = open(FIFO_CLIENT,1)) < 0){
	cout<<"Client "<<a<<": Can't open the connection"<<endl;
	exit(2);
  }
  if( (write(writefd,&b,sizeof(b))) < 0) {
	cout<<"Client "<<b<<": Can't send my pid"<<endl;
	exit(2);
  }
  close(writefd);
}
		/* richiedo al server l'operazione e mando gli operandi al server con questa funzione */
void write_fd_client_operation(int a,float b){
  if( (writefd = open(FIFO_CLIENT,1)) < 0){
	cout<<"Client "<<a<<": Can't open the connection"<<endl;
	exit(3);
  }
  if( (write(writefd,&b,sizeof(b))) < 0) {
	cout<<"Can't send operand"<<endl;
	exit(3);
  }

close(writefd);
}
		/* mi metto in ascolto sulla porta del server per ricevere il output */
float read_fd_output(int a,float b){
  if( (readfd = open(FIFO_SERVER,0)) < 0){
	cout<<"Client "<<a<<": Can't open the connection"<<endl;
	exit(4);
  }  	
  if( (read(readfd,&b,sizeof(b))) < 0) {
	cout<<"Can't send operand"<<endl;
	exit(4);
  }
close(readfd);
return b;
}
