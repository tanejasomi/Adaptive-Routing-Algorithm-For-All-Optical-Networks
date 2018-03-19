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
float l[30][30][41],c[30][30][1],linkut[30][30][41],confree[3000][2];
/* l is the link state matrix. c matrix is used by FAR and LLR to calculate alternate routea */
int n,nooflink;
float meanlinkut;
float b[30],rad;
int bbc=1;
int alpha=1,beta=1,gama=1,delta=1;
int z;  /*no. of nodes in the network*/
int hfar=0; /* for FAR. hfar=0 means 1st route, h=1 means alternate*/
double simclock; /* the simulation clock which event driven */
int wlimit=32;
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
int bblockcount;
float avg_phoplth;
long value[2]={1973272912, 281629770}; /* used by ransom number generators */

/*** global variables used by LLR ******/
int maptrack=0;
int ptr;
int src,destin;

/*struct count
  {
     int destintn;
     int srce;
}x[30];  */
struct xy         //for storing the possible 3 shortest path
 {
 int hoplen;
 int pth[30];
 float weight; //contain nodes in that path
}thr[3],tem,extra,bha;

int counter1;

struct pbkup
 {

      struct xy primary;
        struct xy bkpath[2];
        float totalweight[2];
 }totalpath[3];
int tpath=0;  //total path;

struct count
  {
     int destintn;
     int srce;
}x[30];

 char mat[30][30];
 unsigned int bits[4];
 int nodes,source,dest;
 long count;
 int q[200];
 int cnt=0;
int counter2;

 void graph();
 void pathfind();
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
       int convert;
       char plot[10];
   FILE *fplot;



   printf(" ************************************************* \n");
       printf("    WELCOME TO THE OPTICAL NETWORK SIMULATOR\n");
       printf(" ************************************************* \n");

   printf("Enter name of input file:   ");
   scanf("%s",&file);
   printf("\nEnter filename of the log file:   ");
   scanf("%s",&fname);
   printf("\nEnter filename of the CSV file to be use for plotting:   ");
   scanf("%s",&plot);


   flog=fopen(fname,"w");
   fplot=fopen(plot,"w");


   for(k=0;k<n;k++)
     { x[k].srce=0;
      x[k].destintn=0;}

   fprintf(fplot,"traffic  blockprob  meanlinkut avg_phoplth\n");

  for(meania=1;meania<24;meania+=2){  /* this is to be enabled for fixed load and variable converters run*/
 // for(meania=1;meania<24;meania+=2){/* arrival rates changed to change traffic; this to be enabled for fixed converter and variable load run*/

   fprintf(flog,"*********************************\n");
   fprintf(flog," FOR ARRIVAL RATE = %f UNITS\n",meania);
   fprintf(flog,"*********************************\n\n");
   fprintf(flog,"Sim. Clk   Call no.   Source   Destination   holding time\twavelength\tpath\t\t\tbkup wavelength\tbackup path\n");
   fprintf(flog,"--------   --------   ------   ------------  ------------\t----------\t----\t\t\t---------------\t----------\n\n");


   /* creates the link state matrix*/
    nooflink=0;
   read();  /* reads the file that contains the network topology */
   nodeassign();
  //meania=10;//this to be enabled for fixed load and variable converters run
   convert=0;//this to be enabled for fixed converter and variable load run
   for(i=0;i<n;i++)
       converter[i]=convert;

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
   for(i=0;i<3000;i++)
      for(j=0;j<2;j++)
          confree[i][j]=0;
          counter1=0;
          blockcount=0;
          bblockcount=0;
         avg_phoplth=0;// for all meania loop it shold be reset to zero

   while(simclock<400)
   {
       hfar=0; /* flag used by far to differntiate between initial n alternative route */
       counter=counter+1;
       counter1++;
        printf("\n simclock %lf\n",simclock); printf(" call no. %d\n",counter);
       blockflag=0;
       nobkup=0;
       nopath=0;
      /* printf("enter source\n"); scanf("%d",&source);
       printf("enter destination\n");scanf("%d",&dest); */
 printf("%d",n);

  rad=meania/15;
       //srand(++bbc +time(NULL));   //it is required for getting different s-d pair nos;disabled for fixed s-d pair generation.
        rad=rand()%30+1;
       rad=rad/4 -3;


       do{
          src=nextpoisson(6.3); //if +rad is removed then it will give fixed vlue of strc-dest pair
           }while(src>n-1||src<0);


   //srand(++bbc +2 +time(NULL));
      rad=rand()%25+1;
         rad=rad/4-3;


       do
          {
            do{
             destin=nextpoisson(6.3);//if +rad is removed then it will give fixed vlue of strc-dest pair
             }while(destin>n-1||destin<0);
          }while(src==destin);

 
            x[src].srce++;
          x[destin].destintn++;


          printf("\n%d     %d",src,destin);


       holdtime=holdingtime();
       printf("source is %d destination is %d \n holding time is %f ",src, destin,holdtime);


                      pathfind();
                       routeassign(0);



  /* for(p=0;p<ltrav;p++)
     printf(" the wavelength used is %d\n", wused[p]);*/
      
      logwrite();

            if(wused[0]!=0)
        linkupdate();

        if(counter1==700000)
           break;

           if(counter==500000)
           break;

       nextcall=simclock+arrive();
       printf(" the next call is s'posed to come at %f\n",nextcall);
       depart(nextcall);

       for(i=0;i<n;i++)
       printf("%d ",converter[i]);
   }
      for(i=0;i<n;i++)
     {for(j=0;j<n;j++)
       { if(i!=j)
           {for(w=1;w<=wlimit;w++)
          {
             linkut[i][j][0]+=linkut[i][j][w];
          }
         meanlinkut+=linkut[i][j][0];}
         }}
         meanlinkut=meanlinkut/(wlimit*simclock*nooflink);

      avg_phoplth=avg_phoplth/(counter1-blockcount);   // for avg. primary hoplength calc.

 //   fprintf(flog,"\n----------------------------------------------------\n");
 //   fprintf(flog,"              SUMMARY OF SIMULATION RUN                    \n");
 //  fprintf (flog,"----------------------------------------------------\n");
 //  fprintf(flog,"TOTAL CALLS = %d\nBLOCKED CALLS = %d\n",counter,blockcount);
      blockprob=(double)blockcount/counter1;
 //  fprintf(flog,"BLOCKING PROBABILITY = %lf\n",blockprob);
 //  fprintf(flog,"TRAFFIC = %f units\n\n\n",traffic);
    fprintf(fplot,"%f, %lf, %f %f\n",traffic,blockprob,meanlinkut,avg_phoplth);

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
void pathfind()
  {   int i,j,x,y,k;
      for(i=0;i<n;i++)
         for(j=0;j<n;j++)
          {
             c[i][j][0]=l[i][j][0];


             }

             for(k=0;k<3;k++)
          {for(j=0;j<=thr[k].hoplen;j++)
             thr[k].pth[j]=0;
            thr[k].hoplen=0;
            }
     graph();
     cnt=0;
     tpath=0;



     for(i=0;i<3;i++)
         totalpath[i].primary=thr[i];

/*for(i=0;i<3;i++)
 {
         printf("\n\n\n%d",totalpath[i].primary.hoplen);
         for(j=0;j<=totalpath[i].primary.hoplen;j++)
          printf("\t%d",totalpath[i].primary.pth[j]);
          printf("\n");

}*/
     for(i=0;i<3;i++)
         {
            if(totalpath[i].primary.hoplen>0)

         {
             for(j=0;j<totalpath[i].primary.hoplen;j++)
            {
                x=totalpath[i].primary.pth[j];
                y=totalpath[i].primary.pth[j+1];
               c[x][y][0]=-1;
                c[y][x][0]=-1;
            }

            for(k=0;k<3;k++)
          {for(j=0;j<=thr[k].hoplen;j++)
             thr[k].pth[j]=0;
            thr[k].hoplen=0;
            }
          graph();
          tpath=0;
          cnt=0;



          totalpath[i].bkpath[0]=thr[0];
          totalpath[i].bkpath[1]=thr[1];



       /*   for(k=0;k<2;k++)
            {
                for(j=0;j<=totalpath[i].bkpath[k].hoplen;j++)
                 printf("\t%d",totalpath[i].bkpath[k].pth[j]);
                 printf("\n");
              } */



         //  for(j=0;j<=totalpath[i].bkpath[0].hoplen;j++)
           // printf("\tjatin%d",totalpath[i].bkpath[0].pth[j]);
         for(x=0;x<n;x++)
         for(y=0;y<n;y++)
            c[x][y][0]=l[x][y][0];

          }

       }





 /*  for(i=0;i<3;i++)
     {
         printf("\n\n\n%d",totalpath[i].primary.hoplen);
         for(j=0;j<=totalpath[i].primary.hoplen;j++)
          printf("\t%d",totalpath[i].primary.pth[j]);
          printf("\n");

          for(k=0;k<2;k++)
            {    printf("\t%d",totalpath[i].bkpath[k].hoplen);
                for(j=0;j<=totalpath[i].bkpath[k].hoplen;j++)
                 printf("\t%d",totalpath[i].bkpath[k].pth[j]);
                 printf("\n");
              }


 }*/


  }

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
       // mat=(char**)malloc(n*sizeof(char *));

//       for(i=0;i<n;i++)
       // mat[i]=(char*)malloc(n*sizeof(char));
       // printf("Input the graph matrix :\n");
       printf("\n");
        for(i=0;i<n;i++)
         {for(j=0;j<n;j++)
         {
              if(c[i][j][0]==-1||c[i][j][0]==0)
               mat[i][j]=48;

               if(c[i][j][0]>0||c[i][j][0]<-1)
                   mat[i][j]=49;


         }}

        if(!(n%8))
    temp=n/8;           //n= no. of nodes
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
       int temp[200];
       int i=0,j,k;
       while(i<cnt)
       {
       temp[i]=q[i];
  i++;
       }
       i=cnt-1;
//  printf("%d :",count);
 // printf("   %d    ",cnt-1);
/*while(i>=0)
 {
        printf("%d ",temp[i]);
        i--;
 }
 printf("\n");*/

 for(j=0;j<cnt;j++)
      extra.pth[j]=temp[j];
      extra.hoplen=cnt-1;



 if(tpath<3)
 {
   thr[tpath]=extra;
      tpath++;
   }
else
  {
      for(j=0;j<2;j++)
        for(k=j+1;k<3;k++)
           {
              if(thr[j].hoplen>thr[k].hoplen)
                 {tem=thr[j];
                 thr[j]=thr[k];
                 thr[k]=tem;}
           }
    if(thr[2].hoplen>cnt-1)
       {
          thr[2]=extra;
       }

          for(j=0;j<2;j++)
        for(k=j+1;k<3;k++)
           {
              if(thr[j].hoplen>thr[k].hoplen)
                 {tem=thr[j];
                 thr[j]=thr[k];
                 thr[k]=tem;}
           }
}     }




void routeassign(int track)
{       float maxcost=0,bw=0.25,mean,variance,g,wl;
    int dpath[40],free,wu,m,w_used[10][33],wt[10][10],comb[3],pp=0,bb=0,wb_used[10][15][33];
     int t=14,wavelnb,wavelnp,conv,max[33],maxfree,flag=0;
    int i,j,k,w,a=0,ll;
     struct xy ruts;
     int chosenroute,backroute;
     k=0;

       for(i=0;i<10;i++)
        for(j=0;j<33;j++)
            w_used[i][j]=0;

          for(i=0;i<10;i++)
        for(j=0;j<15;j++)
           for(k=0;k<33;k++)
            wb_used[i][j][k]=0;



  for(i=0;i<3;i++)
    { ruts=totalpath[i].primary;

     for(j=0;j<=ruts.hoplen;j++)
         printf("%d ",ruts.pth[j]);
       wl=0;
        for(w=1;w<=wlimit;w++)
                 {
                 for(j=0;j<ruts.hoplen;j++)
                    {
                 
                      if((l[ruts.pth[j]][ruts.pth[j+1]][w])!=-1)
                          break;
                      
                    }
                    
                    if(j==ruts.hoplen)
                        wl++;
                }     


       for(j=0;j<ruts.hoplen;)
                {
                    for(k=0;k<=wlimit;k++)
                      max[k]=1;
                    for(w=1;w<=wlimit;w++)
                        {
                            if((l[ruts.pth[j]][ruts.pth[j+1]][w])==-1)
                                {
                                   for(m=j+1;m<ruts.hoplen;m++)
                                        {   if((l[ruts.pth[m]][ruts.pth[m+1]][w])==-1)
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
                         w_used[i][k+j]=maxfree;
                      else {
                           for(k=0;k<max[maxfree];k++)
                               w_used[i][k+j]=maxfree;
                           }

                       if(w_used[i][j]==0)
                           conv=1000;
                    if(maxfree==0)
                               j++;
                     else
                          j=j+max[maxfree];
                }

         for(j=1;j<ruts.hoplen;j++)
               {//printf("\nwave=%d",w_used[i][j-1]);
                      if(w_used[i][j-1]!=w_used[i][j])
                      {     //printf("jatin%d",converter[ruts.pth[j]]);
                            if(converter[ruts.pth[j]]>0)
                                 { conv++;
                                  //converter[ruts.pth[j]]--;
                                  }
                            else
                                 conv=1000; }

               }

          for(j=0;j<ruts.hoplen;j++)
             printf("\nwave=%d",w_used[i][j]);

        printf("\n no. of wavelengths converters required are %d",conv);

     //   if(conv<1000)
       //      pp++;     /*pp stands for possible paths*/


     //   printf("\n mean and variance are is %f\n%f",mean,variance);
        g=wl/(sqrt(ruts.hoplen));
      printf("\ng is %f\n",g);
        
         
          totalpath[i].primary.weight=g;
         if(conv>=1000)
            {totalpath[i].primary.weight=1001;
             pp++;}
              conv=0;
}




for(i=0;i<3;i++)
 for(t=0;t<2;t++)
     {  totalpath[i].bkpath[t].weight=0;
        ruts=totalpath[i].bkpath[t];
         if(ruts.hoplen>0)
          {  
              wl=0;
         for(w=1;w<=wlimit;w++)
                 {
                 for(j=0;j<ruts.hoplen;j++)
                    {
                 
                      if((l[ruts.pth[j]][ruts.pth[j+1]][w])!=-1)
                        break;
                      
                    }
                    
                    if(j==ruts.hoplen)
                        wl++;
                }     


       for(j=0;j<ruts.hoplen;)
                {
                    for(k=0;k<=wlimit;k++)
                      max[k]=1;
                    for(w=1;w<=wlimit;w++)
                        {
                            if((l[ruts.pth[j]][ruts.pth[j+1]][w])==-1)
                                {
                                   for(m=j+1;m<ruts.hoplen;m++)
                                        {   if((l[ruts.pth[m]][ruts.pth[m+1]][w])==-1)
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
                         wb_used[i][t][k+j]=maxfree;
                      else {
                           for(k=0;k<max[maxfree];k++)
                               wb_used[i][t][k+j]=maxfree;
                           }

                       if(wb_used[i][t][j]==0)
                           conv=1000;
                    if(maxfree==0)
                               j++;
                     else
                          j=j+max[maxfree];
                }

         for(j=1;j<ruts.hoplen;j++)
               {   //   printf("\nwave=%d",wb_used[i][t][j-1]);
                      if(wb_used[i][t][j-1]!=wb_used[i][t][j])
                        {
                            if(converter[ruts.pth[j]]>0)
                                 { conv++;
                                  //converter[ruts.pth[j]]--;
                                  }
                            else
                                 conv=1000;  }

               }

          for(j=0;j<ruts.hoplen;j++)
              printf("\nwave=%d",wb_used[i][t][j]);

   //      printf("\n no. of wavelengths converters required are %d",conv);

     //   if(conv<1000)
       //      pp++;     /*pp stands for possible paths*/


     //  printf("\n mean and variance are is %f\n%f",mean,variance);
        g=wl/(sqrt(ruts.hoplen));
      printf("\ng is %f\n",g);
       // conv=0;
        
          totalpath[i].bkpath[t].weight=g;
   //  totalpath[i].primary.weight=cost(totalpath[i].primary);
     // printf("%f",totalpath[i].primary.weight);
     //totalpath[i].bkpath[0].weight=cost(totalpath[i].bkpath[0]);
     //totalpath[i].bkpath[1].weight=cost(totalpath[i].bkpath[1]);
}
 else
        totalpath[i].bkpath[t].weight=1000;
  if(conv>=1000)
      totalpath[i].bkpath[t].weight=1000;
    if(totalpath[i].bkpath[t].weight>1000)
       bb++;

          conv=0;
    totalpath[i].totalweight[t]=totalpath[i].primary.weight+totalpath[i].bkpath[t].weight;
    if(totalpath[i].totalweight[t]>1000)
      flag++;

}

        for(i=0;i<3;i++)
           for(j=0;j<2;j++)
          printf("\n%f",totalpath[i].totalweight[j]);


if(pp==3)
    {    chosenroute=0;
      for(j=0;j<20;j++)
                  wused[j]=0;
             printf(" CALL BLOCKED !!! \n ");
              blockcount=blockcount+1;
              blockflag=1;
    }

else if(bb==6||flag==6)
    {
         for(i=0;i<3;i++)
             {
                 if(totalpath[i].primary.weight>maxcost&&totalpath[i].primary.weight<100)
                      {
                           maxcost=totalpath[i].primary.weight;
                           chosenroute=i;

                           }}

   printf(" route is \n");
     for(j=0;j<=totalpath[chosenroute].primary.hoplen;j++)
     printf("%d ",totalpath[chosenroute].primary.pth[j]);
    printf("\n");
    printf("backup path not possible");
    nobkup=1;
    bblockcount++;

    for(j=1;j<totalpath[chosenroute].primary.hoplen;j++)
       { if(w_used[chosenroute][j-1]!=w_used[chosenroute][j])
           {converter[totalpath[chosenroute].primary.pth[j]]--;
           confree[0][0]++;
           k=confree[0][0];
           confree[k][0]=simclock+holdtime;
           confree[k][1]=totalpath[chosenroute].primary.pth[j];

           }}
}


else
 {
     for(i=0;i<3;i++)
         for(k=0;k<2;k++)
             {
                 if(totalpath[i].totalweight[k]>maxcost&&totalpath[i].totalweight[k]<100)
                      {
                           maxcost=totalpath[i].totalweight[k];
                           chosenroute=i;
                           backroute=k;
                           }
                  else if(totalpath[i].totalweight[k]==maxcost)
                        {
                             if(totalpath[i].primary.weight>totalpath[chosenroute].primary.weight&&totalpath[i].primary.weight<100)
                                {  chosenroute=i;
                                   backroute=k; }
                   }        }

   printf(" route is \n");
     for(j=0;j<=totalpath[chosenroute].primary.hoplen;j++)
     printf("%d ",totalpath[chosenroute].primary.pth[j]);
    printf("\n");

     //for(j=0;j<totalpath[chosenroute].primary.hoplen;j++)
     //printf(" the wavelength used is %d \n",w_used[chosenroute][j]);

     for(j=1;j<totalpath[chosenroute].primary.hoplen;j++)
       { if(w_used[chosenroute][j-1]!=w_used[chosenroute][j])
           {converter[totalpath[chosenroute].primary.pth[j]]--;
           confree[0][0]++;
           k=confree[0][0];
           confree[k][0]=simclock+holdtime;
           confree[k][1]=totalpath[chosenroute].primary.pth[j];
           counter2++;
           }
           }


     printf("backup route is \n");
     for(j=0;j<=totalpath[chosenroute].bkpath[backroute].hoplen;j++)
         { printf("%d ",totalpath[chosenroute].bkpath[backroute].pth[j]);
            bpath[j]=totalpath[chosenroute].bkpath[backroute].pth[j];
           wbused[j]=wb_used[chosenroute][backroute][j];}
           printf("\n");

   //    avg_phoplth+=totalpath[chosenroute].primary.hoplen;
    // printf("%d",route[backroute][j]);
     //bpath[j]=route[backroute][j];
    ltravb=j-1;
     //printf("\n");

    // for(j=0;j<totalpath[chosenroute].bkpath[backroute].hoplen;j++)
      //  printf(" the wavelength used is %d \n",wb_used[chosenroute][backroute][j]);

     // for(j=0;route[backroute][j]!=node[track].to;j++)
      //{
       //     w=w_used[backroute][j];
         //   l[route[backroute][j]][route[backroute][j+1]][w]=simclock+holdtime+100;
         //    l[route[backroute][j+1]][route[backroute][j]][w]=simclock+holdtime+100;

    //  }
    for(j=1;j<totalpath[chosenroute].bkpath[backroute].hoplen;j++)
       { if(wb_used[chosenroute][backroute][j-1]!=wb_used[chosenroute][backroute][j])
          { converter[totalpath[chosenroute].bkpath[backroute].pth[j]]--;
           confree[0][0]++;
           k=confree[0][0];
           confree[k][0]=simclock+holdtime;
           confree[k][1]=totalpath[chosenroute].bkpath[backroute].pth[j];
           counter2++;
           }
           }
     //

      //  for(j=0;route[chosenroute][j]!=node[track].to;j++)
        //   wbused[j]=w_used[backroute][j];
          nobkup=0;
}

if(pp!=3)
       {   for(j=0;j<=totalpath[chosenroute].primary.hoplen;j++)
           { wused[j]=w_used[chosenroute][j];
            path[j]=totalpath[chosenroute].primary.pth[j];}
          /** entering the last node also **/
            ltrav=j-1;

         avg_phoplth+=totalpath[chosenroute].primary.hoplen;
              }

       /*for(i=0;i<node[track].routes;i++)
        b[i]=0;*/
     for(j=0;j<=ltrav;j++)
     dpath[j]=path[ltrav-j];
     for(j=0;j<=ltrav;j++)
     path[j]=dpath[j];
    /* for(k=0;k<2;k++)
       { if(totalpath[chosenroute].bkpath[k].weight==1000)
           ll++;  }
       if(ll==2)
        bblockcount--;    */



 //for(i=0;i<node[track].routes;i++)
     //    b[i]=0;
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


   if(nobkup!=1)
   {    i=20-2*ltravb;
       for(p=0;p<ltravb-1;p++)
       fprintf(flog,"w%d",wbused[p]);
       fprintf(flog,"w%-*d",i,wbused[p]);
       //fprintf(flog,"\t");
       p=0;
       fprintf(flog,"%d",bpath[p]);
       while(p<ltravb)
           {   p++;
              fprintf(flog,",");
                  fprintf(flog,"%d",bpath[p]);

                   }
                  // fprintf(flog,"%20d",converter);
    }
    else
      fprintf(flog,"back up doesnot exist");

 }
   else
      fprintf(flog,"blocked");
   fprintf(flog,"\n");

}

/**************************************************************/
/****************** LINK STATUS UPDATE ************************/
/**************************************************************/

/*void linkstatupdate()
{
   int p;

   for (p=0;p<ltrav;p++)
           {
                   l[path[p]][path[p+1]][wused[p]]=simclock+holdtime;
                       l[path[p+1]][path[p]][wused[p]]=simclock+holdtime;
                      // fprintf(flog,"%f %f",l[path[p]][path[p+1]][wused],l[path[p+1]][path[p]][wused]);
           }
   fprintf(flog,"\n");
}*/
/*-----------------------------------------------------------------------------------------------*/
void linkupdate()
{
   int p;

   for (p=0;p<ltrav;p++)
           {
                   l[path[p]][path[p+1]][wused[ltrav-p-1]]=simclock+holdtime;
                       l[path[p+1]][path[p]][wused[ltrav-p-1]]=simclock+holdtime;


                 linkut[path[p]][path[p+1]][wused[ltrav-p-1]]+=holdtime;
                       //   linkut[path[p+1]][path[p]][wused[ltrav-p-1]]+=holdtime;
                     //  fprintf(flog,"%f %f",l[path[p]][path[p+1]][wused],l[path[p+1]][path[p]][wused]);


           }

      if(nobkup!=1){
    for (p=0;p<ltravb;p++)
           {
                   l[bpath[p]][bpath[p+1]][wbused[p]]=-(simclock+holdtime);
                       l[bpath[p+1]][bpath[p]][wbused[p]]=-(simclock+holdtime);

                  linkut[bpath[p]][bpath[p+1]][wbused[p]]+=holdtime;
                        //  linkut[bpath[p+1]][bpath[p]][wbused[p]]+=holdtime;


                     //  fprintf(flog,"%f %f",l[bpath[p]][bpath[p+1]][wused],l[path[p+1]][path[p]][wused]);
           } }

   fprintf(flog,"\n");
}

/****************************************************************/
/*******************  CAll DEPART *******************************/
/****************************************************************/

void depart(float x)
{
   float smallest;
   int i,j,w,k,a;
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

         for(i=0;i<n;i++)
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
        }

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





