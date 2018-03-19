#include<stdio.h>
#include<conio.h>
//#include<alloc.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#define MODLUS 2147483647 /* required for random number generation */
#define MULT1 24112
#define MULT2 26143
#define isdigit(y)(y>=48 && y<=57)
float l[30][30][41],c[30][30][1],linkut[30][30][41],confree[3000][2];;
/* l is the link state matrix. c matrix is used by FAR and LLR to calculate alternate routea */
int n;
float b[30],rad;
float meanlinkut;
int bbc=1,nooflink;
int alpha=1,beta=1,gama=1,delta=1;
int z;  /*no. of nodes in the network*/
int hfar=0; /* for FAR. hfar=0 means 1st route, h=1 means alternate*/
double simclock; /* the simulation clock which event driven */
//int wlimit=4;
int wlimit;
int converter[30];/*no. of wavelengths to be used */ /* source and destination nodes for the connection request */
int wused[33],wbused[33]; /* wavelength used for the lightpath */
double holdtime; ;/* the holding time of the connection request.*/
int ltrav,ltravb; /* the number of links that come in the lightpath to be established*/
int counter=0,blockcount=0; /* counter counts the no. of calls. Blockcount counts the no. of blocked cals */
int blockflag,nobkup,nopath; /* blockflag=1 means call has been blocked. nopath flag is used by FAR and LLR to indicate no more alternate routes are possible */
char fname[15];
char file[15]; /*file names for input and log */
FILE *flog; /* pointer to the log file */

int path[40],bpath[40]; /*stores the links in the route to be used as lightpath*/
float meanht=5; /* mean holdtime to calculate mean traffic */
float meania; /* mean arrival rate of connection requests */
float avg_phoplth;
int convert;
long value[2]={1973272912, 281629770}; /* used by ransom number generators */

/*** global variables used by LLR ******/
int maptrack=0;
int ptr;
//struct map /* data structure that holds the s-d pair and the possible routes betn them */
//`{  int from;
//   int to;
 // int h[40];
 //  int routes;
//} node[1000005];

int src,destin;
float lowmean=1000,stdev;
int hopcount=1000;

/*struct count
  {
     int destintn;
     int srce;
}x[30];  */
struct xy         //for storing the possible 3 shortest path
 {
      float average;
 int hoplen;
 int pth[30];
 //contain nodes in that path
}chosenruts,gantor;
int counter1;

struct pbkup
 {

      struct xy primary;
        struct xy bkpath;

 }totalpath;
int tpath=0;  //total path;

struct count
  {
     int destintn;
     int srce;
}x[30];

 char mat[30][30];
 unsigned int bits[5];
 int nodes,source,dest;
 long count;
 int q[200];
 int cnt=0;


 void graph();
 // void pathfind();
 void prnt();
 void printPaths(int,unsigned int*);



/**************************************/

/*********************************************************/
/********** Main Function Begins *************************/
/*********************************************************/
int main()
{
       int srcdest(),nodeassign(),waveassign(),logwrite();
       int mapcreate(),p;
       int nextpoisson(double);
   void read(),linkstatupdate(),linkupdate(),depart(float),routeassign(int),routeassignmu(int track);
   double arrive(),holdingtime();
   double nextcall;
   float traffic,rad;
   short int mapready;

       int i,j,w,m,k,algo;
       int seed;
       double blockprob,meanprob=0.0;

       char plot[10];
   FILE *fplot;



   printf(" ************************************************* \n");
       printf("    WELCOME TO THE OPTICAL NETWORK SIMULATOR\n");
       printf(" ************************************************* \n");

   
  printf("Enter the max no wavelengths available on each link\n");
   scanf("%d",&wlimit);
   printf("Enter name of input file:   ");
   scanf("%s",&file);
   printf("\nEnter filename which contains the Details:   ");
   scanf("%s",&fname);
   printf("\nEnter the summary filename used for plotting:   ");
   scanf("%s",&plot);


   flog=fopen(fname,"w");
   fplot=fopen(plot,"w");


   for(k=0;k<n;k++)
     { x[k].srce=0;
      x[k].destintn=0;}

   fprintf(fplot,"traffic  blockprob  meanlinkut avg_phoplth  std. deviation\n");

   for(meania=2;meania<21;meania+=2){/* arrival rates changed to change traffic */

   fprintf(flog," \n");
   fprintf(flog,"*********************************\n");
   fprintf(flog," FOR ARRIVAL RATE = %f UNITS\n",meania);
   fprintf(flog,"*********************************\n\n");
   fprintf(flog,"Sim. Clk   Call no.   Source   Destination   holding time\twavelength\tpath\t\t\tbkup wavelength\tbackup path\n");
   fprintf(flog,"--------   --------   ------   ------------  ------------\t----------\t----\t\t\t---------------\t----------\n\n");

   nooflink=0;
   read();  /* reads the file that contains the network topology */
   nodeassign(); /* creates the link state matrix*/


   for(i=0;i<n;i++)
       converter[i]=8;
   seed=8000+5*convert; /* seed fpr random no. generation */
   srand(seed);

   traffic=meania*meanht;

   simclock=0; printf("\n");
   simclock=simclock+arrive();

   for(i=0;i<n;i++)
         for(j=0;j<n;j++)
            for(w=0;w<=wlimit;w++)
            linkut[i][j][w]=0;
         meanlinkut=0;

   for(i=0;i<n;i++)
       converter[i]=8;

        for(i=0;i<3000;i++)
                for(j=0;j<2;j++)
                      confree[i][j]=0;

   counter1=0;
   blockcount=0;  avg_phoplth=0;
   stdev=0;
//   while(simclock<2500)

    while(simclock<100)
   {
       hfar=0; /* flag used by far to differntiate between initial n alternative route */
       counter=counter+1;
       counter1++;  printf("\n simclock %lf\n",simclock); printf(" call no. %d\n",counter);
       blockflag=0;
       nobkup=1;
       nopath=0;
       lowmean=1000;
      /* printf("enter source\n"); scanf("%d",&source);
       printf("enter destination\n");scanf("%d",&dest); */
 printf("%d",n);

       // rad=meania/15;
   // srand(++bbc +time(NULL));
       rad=rand()%30+1;
         rad=rad/4 -3;


       do{
          src=nextpoisson(6.3);
           }while(src>n-1||src<0);


    // srand(++bbc +2 +time(NULL));
        rad=rand()%25+1;
         rad=rad/4-3;


       do
          {
            do{
             destin=nextpoisson(6.3);
             }while(destin>n-1||destin<0);
          }while(src==destin);

          //source-=1;
        // dest-=1;

            x[src].srce++;
          x[destin].destintn++;


      // scanf("\n%d%d",&src,&destin);


       holdtime=holdingtime();
       printf("source is %d destination is %d \n holding time is %f ",src, destin,holdtime);

       /*********CHOOSING ROUTING n WAVELENGTH ASSIGNMENT ALGOS*************/

          /**** searching the node pair in the map *****/
                  // for(i=0,mapready=0;i<=maptrack;i++)
                   //{
                     //if((node[i].from==source && node[i].to==dest)||(node[i].from==dest && node[i].to==source))
                   //mapready=1; /**** the node pair found in the map ***/
                     //  break;
                   //}
                   /**** if the source n destination are already on the map ******/
                   //if(mapready)
                   //routeassign(i); /**** go directly to choosing the least loaded route ***/
                   //else /*** first the node pairs need to be added to the map ***/
                   //{
                   /****** creating duplicate network *****/
                      for(i=0;i<n;i++)
                  for(j=0;j<n;j++)
                     {
                        c[i][j][0]=l[i][j][0];

                      }
                      graph();
                      routeassign(0);



  /* for(p=0;p<ltrav;p++)
     printf(" the wavelength used is %d\n", wused[p]);*/
      logwrite();

            if(wused[0]!=0)
        linkupdate();

        if(counter==500000)
           break;



       nextcall=simclock+arrive(); printf(" the next call is s'posed to come at %f\n",nextcall);
       depart(nextcall);
   }

 for(i=0;i<n;i++)
     {for(j=0;j<n;j++)
       { if(i>j)
           {for(w=1;w<=wlimit;w++)
          {
             linkut[i][j][0]+=linkut[i][j][w];
          }
          linkut[i][j][0]/=(wlimit*simclock);
         meanlinkut+=linkut[i][j][0];}
         }}
         meanlinkut=meanlinkut/(nooflink);
       
          for(i=0;i<n;i++)
     {for(j=0;j<n;j++)
       { if(i>j&&l[i][j][0]>0)
          stdev+=(linkut[i][j][0]-meanlinkut)*(linkut[i][j][0]-meanlinkut);
          }}
       stdev=stdev/nooflink;
       stdev=sqrt(stdev);
  avg_phoplth=avg_phoplth/(counter1-blockcount);   // for avg. primary hoplength calc.

   fprintf(flog,"\n----------------------------------------------------\n");
   fprintf(flog,"              SUMMARY OF SIMULATION RUN                    \n");
   fprintf (flog,"----------------------------------------------------\n");
   fprintf(flog,"TOTAL CALLS = %d \nBLOCKED CALLS = %d\n",counter,blockcount);
   blockprob=(double)blockcount/counter1;
   fprintf(flog,"BLOCKING PROBABILITY = %lf\n",blockprob);
   fprintf(flog,"TRAFFIC = %f units\n\n\n",traffic);
   fprintf(fplot,"%f, %lf, %f %f    %f\n",traffic,blockprob,meanlinkut,avg_phoplth,stdev);


    fprintf(flog,"\n node\tsource\tdestination");
   for(k=0;k<n;k++)
   fprintf(flog,"\n %d\t%d\t%d",k,x[k].srce,x[k].destintn);
    if(counter==500000)
           break;
}


   fprintf(fplot,"\n %d, %d, \n",n,nooflink);

   fclose(flog);
   fclose(fplot);
   getch();
}


 /**********************************************************/
 /*************** INPUT FILE READING ***********************/
 /**********************************************************/

 void read()
{
       int i,j,k,neg=0;
       int row,col;
   char c='\0';
       int flag;

       FILE *f;
       f = fopen(file,"r");

       i = 0;
       j = 0;
       flag = 0;
       while(c!=EOF)
       {
               c = getc(f);

           if(flag)
           {
               if(isdigit(c))
                       k = k*10 + ((int)c-48);

               else if(c==',')
               {
                       if(neg)
                               l[i][j][0]      = 0 - k;
                       else
                               l[i][j][0] = k;
                       j++;
                       flag = 0;
                       neg = 0;
               }

               else if(c=='\n'||c==EOF)
               {
                       if(neg)
                               l[i][j][0]      = 0 - k;
                       else
                               l[i][j][0] = k;

                       if(i>0&&col!=j+1)
                       {
                           printf("Error !\n");
                           printf("No. of elements in rows not same\n");
                           break;
                       }

                       col = j+1;
                       j = 0;
                       i++;
                       flag = 0;
                       neg = 0;
               }

               else
               {

                       printf("Error !\n");
                       printf("Bad entry in file.\n");
                       break;
               }
           }

           else
           {
               if(c==' '||c=='\t')
                       continue;

               else if(c=='-')
                       neg = 1;

               else if(isdigit(c))
               {
                       k = (int)c-48;
                       flag = 1;
               }

               else
               {

                       printf("Error !\n");
                       printf("Bad entry\n");
                       break;
               }
           }
       }

   row = i;
       n=row;
for(i=0;i<n;i++)
           for(j=0;j<n;j++)
           {
        if(i!=j)
        {if(l[i][j][0]>0)
            nooflink++;
 }        }
nooflink=nooflink/2;
}

/*******************************************************************/
/****************** STATE VARIABLE INITIALIZE **********************/
/*******************************************************************/

int nodeassign()

{
 int i,j,w;

       for(i=0;i<n;i++)
       {
               for(j=0;j<n;j++)
               {
                       for(w=1;w<=wlimit;w++)
                       {
                               if(i==j || l[i][j][0]==-1) /* if no path exists or for diagonal elements */
                                       l[i][j][w]=-2;

                               else
                               {
                                       if(i != j && l[i][j][0]==0)
                                               l[i][j][w]=-1; /* -1 means available */
                                       else
                                               l[i][j][w]=-1;
                               }
                       }
               }
       }

   return 0;

}

/****************************************************************/
/********************** CALL ARRIVE *****************************/
/****************************************************************/

float arrive()
{
   double u;
   float e;
   double urand(int);
   float nxtarrive;

   u=urand(1);

   e= -(1/meania)*log(u);
   //printf("e is %f \n",e);
   //nxtarrive=(float)e;
   printf("next call is after %f \n",e);
   return(e);
}

/****************************************************************/
/******************** Source/Destination ***********************/
/****************************************************************/

int srcdest()
{
   double x,r;
   int y;


   r=(double)rand()/(double)(RAND_MAX+1);
   x=r*n;
   y=(int)x;
   return(y);
}

/**************************************************************/
/********************   Holding Time *************************/
/************************************************************/


float holdingtime()
{
   float htime;
   double u; /* u(0,1) */
   //double e;

   double urand(int);
   do{
   u=urand(0);
   htime = -meanht*log(1-u);
   }
   while(htime==0);
   return(htime);
}




/************************************************************/
          /* FUNCTION FOR MAPPING */
/***********************************************************/
/***********************************************************/





void push(int i)
       {
       if(cnt>=200)
       {
       printf("Cannot Push");
       return;
       }
       q[cnt]=i;
       cnt++;
       }
       int pop()
       {
       int lop;
       if(cnt<0)
       {
       printf("Cannot Pop");
       }
       cnt--;
       lop=q[cnt];
       return lop;
       }
void graph()
{
        int temp;
        int i;
   int j;
   int s,d;
    s=src;
    d=destin;
        nodes=n;
        source=s;
        dest=d;
        count=0;
        cnt=0;
       // mat=(char**)malloc(n*sizeof(char *));

       // for(i=0;i<n;i++)
//       mat[i]=(char*)malloc(n*sizeof(char));
       // printf("Input the graph matrix :\n");
        for(i=0;i<n;i++)
         {for(j=0;j<n;j++)
         {
              if(c[i][j][0]==-1||c[i][j][0]==0)
               mat[i][j]=48;

               if(c[i][j][0]>0||c[i][j][0]<-1)
                   mat[i][j]=49;


         }     }



 //             for( i=0;i<n;i++)`
       //  for(j=0;j<n;j++)
         //   printf("%c",mat[i][j]);


   //   scanf(" %c",&mat[i][j]);

     //read();
        if(!(n%8))temp=n/8;           //n= no. of nodes
        else temp=n/8+1;

       // bits = (unsigned int*)malloc(temp*sizeof(int)); //No of integers used for bits
       memset(bits,0,sizeof(bits)); //set all bits to zero


        temp = source/8;//set the source bit to zero
        bits[temp]|=(0x1<<(source%8));

        push(source);
//       printf("All paths from %d to %d are(In reverse order)\n",source,dest);
        printPaths(source,bits);
}

void printPaths(int s,unsigned int*bs)
{
int i;
 for( i=0;i<nodes;i++)
 {

        if(i!=s && mat[s][i]=='1')
        {
               int temp = i/8;

               unsigned int temp1 = bs[temp]>>(i-8*temp);

               if(i==dest)
               {
                 count++;
                 push(i);
                 prnt();
                 pop();
               }

               else if(!(temp1 & 0x1))
               {
                 push(i);
                 bs[temp]=bs[temp]|(0x1<<(i-8*temp));//set bit
                 printPaths(i,bs);
                 bs[temp]=bs[temp]&(~(0x1<<(i-8*temp)));//unset bit after returning from recursion
                 pop();
               }
        }
 }
}

void prnt()
{
       int temp[200]={0,0};
       int i=0,j,k,w,wavelnb=0,wavelnp=0,wl[33];
   float mean=0;
        struct xy ruts,tem;
        //lowmean=1000;
          for(i=0;i<20;i++)
    ruts.pth[i]=0;
    ruts.hoplen=0;
    i=0;
       while(i<cnt)
       {
       temp[i]=q[i];
  i++;
       }
       i=cnt-1;
/*        
 printf("%d :",count);
 printf("   %d    ",cnt-1);
 while(i>=0)
 {
        printf("%d ",temp[i]);
        i--;
 }
 printf("\n");*/



 for(j=0;j<cnt;j++)
      ruts.pth[j]=temp[j];
      ruts.hoplen=cnt-1;

      for(j=0;j<ruts.hoplen;j++)
             {
                  for(w=1;w<=wlimit;w++)
                   {
                      if((l[ruts.pth[j]][ruts.pth[j+1]][w])==-1)
                          continue;
                      else if((l[ruts.pth[j]][ruts.pth[j+1]][w])<0)
                          wavelnb++;
                       else
                           wavelnp++;
                    }

                 //   printf("\n%d  %d",wavelnb,wavelnp);
                 // if(wavelnp+wavelnb==wlimit)
                    // { printf("this path not possible");
                      // break; }
                  wl[j]=wavelnp+wavelnb;

                  mean=mean+wl[j];
                   wavelnp=0;
                   wavelnb=0;
                   }
       // if(wavelnp+wavelnb==wlimit)
            // continue;
             mean=mean/j;
             ruts.average=mean;

          //  printf(" mean =%f",mean);

             wavelnp=0;
                  wavelnp=0;

       if(mean<lowmean)
           {
              lowmean=mean;
              chosenruts=ruts;
              hopcount=ruts.hoplen;

              }

       else if(mean==lowmean)
        {    if(hopcount>ruts.hoplen)
                {

                    chosenruts=ruts;
                    hopcount=ruts.hoplen;
                    }}

}



void routeassign(int track)
{     float mincost=1000,bw=0.25,mean,variance,g,wl[33];
    int free,wu,m,w_used[33],wt[10][10],comb[3],pp=0,bb=0,wb_used[10][15][33];
     int t=14,wavelnb,wavelnp,conv=0,max[33],maxfree;
    int i,j,k,w,a=0;
     struct xy ruts;
     int chosenroute,backroute;
     k=0;

     int dpath[40];
     int p; /*address pointers*/
   int wsum=0;
   w=1;


        for(j=0;j<33;j++)
            w_used[j]=0;


     

     for(j=0;j<chosenruts.hoplen;)
            {
                    for(k=0;k<=wlimit;k++)
                      max[k]=1;
                    for(w=1;w<=wlimit;w++)
                        {
                            if((l[chosenruts.pth[j]][chosenruts.pth[j+1]][w])==-1)
                                {
                                   for(m=j+1;m<chosenruts.hoplen;m++)
                                        {   if((l[chosenruts.pth[m]][chosenruts.pth[m+1]][w])==-1)
                                              max[w]++;
                                             else break;
                                              }
                                 }
                             else
                               max[w]=-1;

                          //printf("\n%d",max[w]);
                          }
                      maxfree=0;
                      max[0]=0;
                      for(w=0;w<wlimit;w++)
                          {
                                if(max[w+1]>max[maxfree])
                                  maxfree=w+1;
                          }
                      //  printf("\n%d",maxfree);
                      if(maxfree==0)
                        w_used[k+j]=maxfree;
                      else {
                           for(k=0;k<max[maxfree];k++)
                              w_used[k+j]=maxfree;
                           }

                       if(w_used[j]==0)
                             conv=1000;

                    if(maxfree==0)
                               j++;
                     else
                          j=j+max[maxfree];
                }

         for(j=1;j<chosenruts.hoplen;j++)
               {   //   printf("\nwave=%d",wb_used[i][t][j-1]);
                      if(w_used[j-1]!=w_used[j])
                        {
                            if(converter[chosenruts.pth[j]]>0)
                                 { conv++;
                                  //converter[ruts.pth[j]]--;
                                  }
                            else
                                 conv=1000;  }

               }
         // for(j=0;j<chosenruts.hoplen;j++)
             // printf("wave=%d,",w_used[j]);

         printf("\n no. of wavelengths converters required are %d",conv);
if(conv>=1000)
       {
           printf("\n *********************************************** \n");
       printf("\n !!!!!! THE CALL HAS BEEN BLOCKED !!!!!!\n ");
       printf("\n *********************************************** \n");
       wused[0]=0;
       blockcount=blockcount+1;
       blockflag=1;
       nobkup=1;
   }

   else
       {

         totalpath.primary=chosenruts;
         printf("wavelength used is\n");
          for(p=0;p<chosenruts.hoplen;p++)
            {wused[p]=w_used[p];
            //  wused[p]=w-1;
              printf("%d  ",w_used[p]);}
              ltrav=chosenruts.hoplen;  
           printf("primary path is\n");

        avg_phoplth+=totalpath.primary.hoplen;
          for(p=0;p<=totalpath.primary.hoplen;p++)
             { path[p]=totalpath.primary.pth[p];
              printf("%d ",path[p]);}

              for(p=1;p<chosenruts.hoplen;p++)
                        {   if(w_used[p-1]!=w_used[p])
                               {converter[chosenruts.pth[p]]--;
                                  confree[0][0]++;
                                 k=confree[0][0];
                              confree[k][0]=simclock+holdtime;
                                 confree[k][1]=chosenruts.pth[p];

        }                         }   }

    


       for(i=0;i<3;i++)
        b[i]=0;
     for(j=0;j<=ltrav;j++)
     dpath[j]=path[ltrav-j];
     for(j=0;j<=ltrav;j++)
     path[j]=dpath[j];



 for(i=0;i<3;i++)
         b[i]=0;
         z=0;

        //add the hop length of current primary path to the privious sum
}

/*********************************************************************/
/**********************************************************/
/********************* LOGWRITE ***************************/
/**********************************************************/

int logwrite()

{


   int p,i;
   fprintf(flog,"%f\t%d\t %d\t     %d\t      %f\t\t",simclock,counter,src,destin,holdtime);
   if(blockflag!=1)
   {    i=18-2*ltrav;
       for(p=(ltrav-1);p>=1;p--)
       fprintf(flog,"w%d",wused[p]);
        fprintf(flog,"w%-*d",i,wused[p]);
       fprintf(flog,"\t");
       p=ltrav;
       fprintf(flog,"%d",path[p]);
       while(p>1)
           {
              fprintf(flog,",");
                  fprintf(flog,"%d",path[p-1]);
                          p--;
                   }
                      i=20-2*ltrav;
                    fprintf(flog,",%-*d",i,path[p-1]);
                  // fprintf(flog,"%20d",converter);


 }
   else
      fprintf(flog,"blocked");
   fprintf(flog,"\n");

}

/**************************************************************/
/****************** LINK STATUS UPDATE ************************/
/**************************************************************/

void linkupdate()
{
   int p;

   for (p=0;p<ltrav;p++)
           {
                   l[path[p]][path[p+1]][wused[ltrav-p-1]]=simclock+holdtime;
                       l[path[p+1]][path[p]][wused[ltrav-p-1]]=simclock+holdtime;
                       linkut[path[p]][path[p+1]][wbused[ltrav-p-1]]+=holdtime;
                         linkut[path[p+1]][path[p]][wbused[ltrav-p-1]]+=holdtime;
                     //  fprintf(flog,"%f %f",l[path[p]][path[p+1]][wused],l[path[p+1]][path[p]][wused]);

           }

   fprintf(flog,"\n");
}

/****************************************************************/
/*******************  CAll DEPART *******************************/
/****************************************************************/

void depart(float x)
{
   float smallest;
   int i,j,w,k;
   int printed;

   do{
   smallest=10000;
   /* checking if there are calls to be terminated before the next call*/
   for(i=0;i<n;i++)
   {
       for(j=0;j<n;j++)
       {
           if(i<j)
           {
               for(w=1;w<=wlimit;w++)
               {
                   if((l[i][j][w]<smallest)&&(l[i][j][w]>-1))
                          smallest=l[i][j][w];
               }
           }
       }
   }



if(x>=smallest) /* certain calls require termination before next call arrives */
   {
       simclock=smallest;
       printed=0;
       for(i=0;i<n;i++)
       {
           for(j=0;j<n;j++)
           {
               if(i<j)
               {
                   for(w=1;w<=wlimit;w++)
                   {
                       if(l[i][j][w]==smallest)
                       {
                           l[i][j][w]= -1;
                           l[j][i][w]=-1;




                           if (printed==0){
                           fprintf(flog,"\n%f  ",simclock);
                           fprintf(flog,"Wavelengths freed:");
                           printed=1;}

                           fprintf(flog," w%d on L%d-%d, ", w,i,j);

                        }
                    }
                }
            }
        }

      /*   for(i=0;i<n;i++)
       {
           for(j=0;j<n;j++)
           {
               if(i<j)
               {
                   for(w=1;w<=wlimit;w++)
                   {
                       if(l[i][j][w]==-smallest)
                       {
                           l[i][j][w]= -1;
                           l[j][i][w]=-1;


                           fprintf(flog,"  w%d on L%d-%d, ", w,i,j);

                        }
                    }
                }
            }
        }*/
          for(i=1;i<=confree[0][0];i++)
           {
              if(confree[i][0]==smallest)
                {k=confree[i][1];
                  converter[k]++;
                   confree[0][0]--;
                    }
                 }
    fprintf(flog,"\n\n");
    }
   }
   while(x>=smallest);
   if(x<smallest)
       simclock=x; printf(" simclock is %f as there are no departing calls\n",simclock);


}

/************* FUNCTION FOR U(0,1) *******************/

double urand(int c)
{
       long zi, lowprd,hi31;
       double x;
       int i;


             zi=value[c];
             lowprd=(zi&65535)*MULT1;
             hi31=(zi>>16)*MULT1+(lowprd>>16);
             zi=((lowprd&65535)-MODLUS)+((hi31&32767)<<16)+(hi31>>15);
             if(zi<0) zi+=MODLUS;


             lowprd=(zi&65535)*MULT2;
             hi31=(zi>>16)*MULT2+(lowprd>>16);
             zi=((lowprd&65535)-MODLUS)+((hi31&32767)<<16)+(hi31>>15);
             if(zi<0) zi+=MODLUS;
             value[c]=zi;
             x=(zi>>7|1)/16777216.0;
             //printf("x is %f\n",x);
             return(x);


}


int nextpoisson(double lambda)
 {


    double nlambda=exp(-lambda);
   //printf("%lf",lambda);
   double product = 1;
       int count =  0;
        int result=0,n=25;
       while (product >= (nlambda)) {
               product *=  ((double)rand() / ((double)(RAND_MAX)+(double)(1)) );

               //printf("%lf ",product);
               result = count;
               count++; // keep result one behind
               }
       return result;

       }







