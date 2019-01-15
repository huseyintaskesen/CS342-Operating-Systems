 
#include <stdio.h>
#include <stdlib.h>

#define CYLINDERS 5000
#define MAXREQUESTS 1000

long long int start = 0;

 long long int request_array[MAXREQUESTS+50000];

int min(int a,int b){
	if(a<b){
		return a;
	}
	return b;
}
long long int* sorter() {

	long long int temp = 0, i = 0, j = 0;

	for (i = 0; i < MAXREQUESTS; ++i) {

       	for (j = i + 1; j < MAXREQUESTS; ++j) {

            if (request_array[i] > request_array[j]) {

                temp =  request_array[i];
                request_array[i] = 	request_array[j];
                request_array[j] = temp;
            }
        }
    }

    return request_array;
}

long long int First_come_first_serve(long long int *request_array) {

	long long int i = 0, headMovt = 0, this_start = start;

  
    for(i = 0; i < MAXREQUESTS; i++) {

    	headMovt += min(llabs(this_start - request_array[i]),llabs(this_start+CYLINDERS-request_array[i]));
    	this_start=request_array[i];
    }
        
    return llabs(headMovt);
}


long long int sstf(long long int * request_array) {

	int currHead=start;
	long long int ans=0;
	while(1==1){
		int mnd=1000000000;
		int mni=-1;
		for(int j=0;j<MAXREQUESTS;j++){
			if(request_array[j]!=-1){
				if(llabs(request_array[j]-currHead)<mnd){
					mnd=llabs(request_array[j]-currHead);
					mni=j;
				}
			}

		}
		if(mni!=-1){
			ans+=mnd;
			currHead=request_array[mni];
			request_array[mni]=-1;
		}else{
			break;
		}
	}
	return ans;
}

long long int scan(long long int * reqArr) {

	int hash[CYLINDERS+400]={0};
	for(int i=0;i<MAXREQUESTS;i++){
		hash[reqArr[i]]=1;
	}
	long long int ans=0;
	for(int j=start-1;j>=0;j--){
		if(hash[j]==1){
			
			start=j;
		}
		ans++;
	}
	for(int j=0;j<CYLINDERS;j++){
		ans++;
	}

return llabs(ans);

}


long long int c_scan(long long int * reqArr) {
     int queue[CYLINDERS],n,head,i,j,k,seek=0,max,diff,temp,queue1[MAXREQUESTS],queue2[MAXREQUESTS],
                        temp1=0,temp2=0;
            float avg;
            max=CYLINDERS;
            head=start;
            n=MAXREQUESTS;
            for(i=0;i<n;i++)
            {
            		temp=reqArr[i];
                        if(temp>=head)
                        {
                                    queue1[temp1]=temp;
                                    temp1++;
                        }
                        else
                        {
                                    queue2[temp2]=temp;
                                    temp2++;
                        }
            }

            
            for(i=0;i<temp1-1;i++)
            {
                        for(j=i+1;j<temp1;j++)
                        {
                                    if(queue1[i]>queue1[j])
                                    {
                                                temp=queue1[i];
                                                queue1[i]=queue1[j];
                                                queue1[j]=temp;
                                    }
                        }
            }
            for(i=0;i<temp2-1;i++)
            {
                        for(j=i+1;j<temp2;j++)
                        {
                                    if(queue2[i]>queue2[j])
                                    {
                                                temp=queue2[i];
                                                queue2[i]=queue2[j];
                                                queue2[j]=temp;
                                    }
                        }
            }
            for(i=1,j=0;j<temp1;i++,j++)
            queue[i]=queue1[j];
            queue[i]=max;
            queue[i+1]=0;
            for(i=temp1+3,j=0;j<temp2;i++,j++)
            queue[i]=queue2[j];
            queue[0]=head;
            for(j=0;j<=n+1;j++)
            {
                        diff=llabs(queue[j+1]-queue[j]);
                        seek+=diff;
            }
         
            return seek;
}

long long int look(long long int* reqArr) {
int LOW=0;int HIGH=CYLINDERS;

int queue[CYLINDERS], head, q_size, i,j, seek=0, diff, max, temp, queue1[CYLINDERS], queue2[CYLINDERS], temp1=0, temp2=0;
  float avg;

  q_size=MAXREQUESTS;

  head=start;
  
  for(i=0; i<q_size; i++){
  	temp=reqArr[i];
    //queue1 - elems greater than head
    if(temp >= head){
        queue1[temp1] = temp;
        temp1++;
    } else {
        queue2[temp2] = temp;
        temp2++;
    }
  }

  //sort queue1 - increasing order
  for(i=0; i<temp1-1; i++){
   for(j=i+1; j<temp1; j++){
       if(queue1[i] > queue1[j]){
         temp = queue1[i];
         queue1[i] = queue1[j];
         queue1[j] = temp;
       }
    }
  }

  //sort queue2 - decreasing order
  for(i=0; i<temp2-1; i++){
    for(j=i+1; j<temp2; j++){
      if(queue2[i] < queue2[j]){
          temp = queue2[i];
          queue2[i] = queue2[j];
          queue2[j] = temp;
      }
    }
  }

  if(llabs(head-LOW) >= llabs(head-HIGH)){

      for(i=1,j=0; j<temp1; i++,j++){
          queue[i] = queue1[j];
      }

      for(i=temp1+1, j=0; j<temp2; i++, j++){
          queue[i] = queue2[j];
      }

  } else {

      for(i=1,j=0; j<temp2; i++,j++){
          queue[i] = queue2[j];
      }

      for(i=temp2+1, j=0; j<temp1; i++, j++){
          queue[i] = queue1[j];
      }

  }

  queue[0] = head;

  for(j=0; j<q_size; j++){
      diff=llabs(queue[j+1] - queue[j]);
        seek += diff;
       

  }

return seek;
}

long long int c_look(long long int* reqArr) {
int LOW=0;int HIGH=CYLINDERS;
 int queue[CYLINDERS], head, q_size, i,j, seek=0, diff, max, min, range, temp, queue1[CYLINDERS], queue2[CYLINDERS], temp1=0, temp2=0;
  float avg;

  q_size=MAXREQUESTS;
  head=start;

  for(i=0; i<q_size; i++){
    temp=reqArr[i];
    if(temp >= head){
      queue1[temp1] = temp;
      temp1++;
    } else {
      queue2[temp2] = temp;
      temp2++;
    }
  }


  //sort queue1 - increasing order
  for(i=0; i<temp1-1; i++){
    for(j=i+1; j<temp1; j++){
      if(queue1[i] > queue1[j]){
        temp = queue1[i];
        queue1[i] = queue1[j];
        queue1[j] = temp;
      }
    }
  }

  //sort queue2
  for(i=0; i<temp2-1; i++){
    for(j=i+1; j<temp2; j++){
      if(queue2[i] > queue2[j]){
        temp = queue2[i];
        queue2[i] = queue2[j];
        queue2[j] = temp;
      }
    }
  }

  if(llabs(head-LOW) <= llabs(head-HIGH)){

    for(i=1,j=temp2-1; j>=0; i++,j--){
        queue[i] = queue2[j];
    }

    queue[i] = LOW;
    queue[i+1] = HIGH;

    for(i=temp2+3,j=temp1-1; j>=0; i++,j--){
        queue[i] = queue1[j];
    }

  } else {

    for(i=1,j=0; j<temp1; i++,j++){
        queue[i] = queue1[j];
    }

    queue[i] = HIGH;
    queue[i+1] = LOW;

    for(i=temp1+3,j=0; j<temp2; i++,j++){
        queue[i] = queue2[j];
    }


  }
  queue[0] = head;

  for(j=0; j<q_size; j++){
    diff=llabs(queue[j+1] - queue[j]);
    seek += diff;
   
  }
return seek;
}




int main ( int argc, char *argv[]) {


	long  long int i = 0;
	start = atoi(argv[1]); //initial head position
	freopen(argv[2], "r", stdin);  //getting  input from file 

	if(argc!=3){
	for(i = 0; i < MAXREQUESTS; i++) {

		request_array[i] = rand() % 5000; //generating random MAXREQUESTS for head movement
	}
}else{
	for(i = 0; i < MAXREQUESTS; i++) {

	scanf("%lld",&request_array[i]);
	scanf("%lld",&request_array[i]);
	}
}
	printf("\nStart index: %lld\n\n", start);
	printf("First_come_first_serve  = %lld\n", First_come_first_serve(request_array));
	printf("SSTF  = %lld\n", sstf(request_array));
	printf("SCAN = %lld\n", scan(request_array));
	printf("c_scan = %lld\n", c_scan(request_array));
	printf("LOOK = %lld\n", look(request_array));
	printf("C-LOOK = %lld\n\n", c_look(request_array));

	return 0;
}
