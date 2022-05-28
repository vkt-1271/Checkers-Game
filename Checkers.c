#include <stdio.h>
#include <unistd.h>
#include <omp.h>
#include <time.h>

#define N 8

// player check for win/loss conditions
int plcheck(int a[N][N])
{
	int x=1;
	#pragma omp parallel
    		#pragma omp parallel for
    			for(int i=1;i<N;i++)
				{
	    			#pragma omp parallel for
	    				for (int j=0; j<N; j++)
	    				{
		    				if (a[i][j] == 3)
		    				{
		        				if (j == 0)
		        				{
		            					if (a[i-1][j+1] == 1 || (i>1 && a[i-2][j+2] == 1))
		            					{
		                					x=0;
		                				}
		                		}
		        				if(j == 7)
		        				{
		            				if (a[i-1][j-1] == 1 || (i>1 && a[i-2][j-2] == 1))
		                			{	
		                				x=0;
		                			}
		                		}
		        				else if(a[i-1][j-1] == 1 || a[i-1][j+1] == 1 || (i>1 && a[i-2][j-2] == 1) || (i>1 && a[i-2][j+2] == 1))
		        				{
		            				x=0;
		            			}
							}
						}
				}
    return x;
}

// Player Moves

void plmoveR(int a[N][N],int i,int j)		// move Right
{ 
    a[i][j] = 1;
    a[i-1][j+1] = 3;
}

void plmoveJR(int a[N][N],int i,int j)	 // move Jump Right
{
    a[i][j] = 1;
    a[i-1][j+1] = 1;
    a[i-2][j+2] = 3;
}

void plmoveL(int a[N][N],int i,int j)		// move Left
{
    a[i][j] = 1;
    a[i-1][j-1] = 3;
}

void plmoveJL(int a[N][N],int i,int j)	 // move Jump Left
{
    a[i][j] = 1;
    a[i-1][j-1] = 1;
    a[i-2][j-2] = 3;
}


// Machine check win or loss condition
int mcheck(int a[N][N])
{
	int x = 1;
	#pragma omp parallel
    		#pragma omp parallel for
    			for (int i=0; i<7; i++)
    			{
    				#pragma omp parallel for
        				for (int j=0; j<N; j++)
        				{
							if (a[i][j] == 2)
							{
								if (j == 0)
								{
									if (a[i+1][j+1] == 1 || (i<6 && a[i+2][j+2] == 1))
									{
										x=0;
									}
								}
								else if (j == 7)
								{
										if (a[i+1][j-1] == 1 || (i<6 && a[i+2][j-2] == 1))
										{
											x=0;
										}
								}
								else if (a[i+1][j-1] == 1 || a[i+1][j+1] == 1 || (i<6 && a[i+2][j-2] == 1) || (i<6 && a[i+2][j+2] == 1))
								{
										x=0;
								}
							}
                    	}
                }
    return x;
}

// Machine check possible move
void AIcheckR(int a[N][N],int ai[N][N],int i,int j)
{
	if (a[i+1][j+1] == 1)
	{
		ai[i][j] = 31;  // safe - safe
		if (i<6 && j<6 && (a[i+2][j+2] == 3 || (a[i+2][j] == 3 && a[i][j+2] == 1)))
			ai[i][j] = 11; // safe - kill
	}
   	else if(i<6 && j<6 && a[i+1][j+1] == 3 && a[i+2][j+2] == 1)
   	{
		ai[i][j] = 41; // kill - safe
		if (i<5 && j<5 && (a[i+3][j+3] == 3 || (a[i+3][j+1] == 3 && a[i+1][j+3] == 1)))
			ai[i][j] = 21; // kill- kill
	}
}

void AIcheckL(int a[N][N],int ai[N][N],int i,int j)
{
	if (a[i+1][j-1] == 1)
	{
		ai[i][j] = 32;  // safe - safe
		if (i<6 && j>1 && (a[i+2][j-2] == 3 || (a[i+2][j] == 3 && a[i][j-2] == 1)))
			ai[i][j] = 12; // safe - kill
	}
    else if (i<6 && j>1 && a[i+1][j-1] == 3 && a[i+2][j-2] == 1)
    {
		ai[i][j] = 42; // kill - safe
		if (i<5 && j>2 && (a[i+3][j-3] == 3 || (a[i+3][j-1] == 3 && a[i+1][j-3] == 1)))
			ai[i][j] = 22; // kill- kill
	}
}


// Machine Moves
void moveR(int a[N][N],int i,int j)	// move Right
{
    a[i][j] = 1;
    a[i+1][j+1] = 2;
}

void moveJR(int a[N][N],int i,int j)	// move Jump Right
{
    a[i][j] = 1;
    a[i+1][j+1] = 1;
    a[i+2][j+2] = 2;
}

void moveL(int a[N][N],int i,int j)	// move Left
{
    a[i][j] = 1;
    a[i+1][j-1] = 2;
}

void moveJL(int a[N][N],int i,int j) // move Jump Left
{
    a[i][j] = 1;
    a[i+1][j-1] = 1;
    a[i+2][j-2] = 2;
}

void display(int arr[N][N])
{
	#pragma omp for
	for (int i=0; i<N; i++)
	{
		for (int j=0; j<N; j++)
		{
			if(arr[i][j]==2)
				printf("B ");
			else if(arr[i][j] == 3)
				printf("W ");
			else if(arr[i][j]==0)
				printf("* ");
			else
				printf("- ");
		}
		printf("\n");
	}
		
}


int main()
{
	int a[N][N];
    
    	// # 0 for block
    	// # 1 for posibile move path 
    	// # 2 for machine "B"
    	// # 3 for Player "W"

	#pragma omp parallel
    		#pragma omp parallel for
    			for (int i=0; i<N; i++)
    			{	
    				#pragma omp parallel for
        				for (int j=0; j<N; j++)
        				{
            					if((i+j)%2 != 0)
                					if (i<3)
                    						a[i][j] = 2;
                				else if (i>4)
                    					a[i][j] = 3;
                					else
                    					a[i][j] = 1;
            					else
                				a[i][j] = 0;
        				}
        			}
        		

    	// Total remaning players
    	// l1 Player1
    	// l2 Machine
    	int l1 = 12,l2 = 12;
    	while (1)
		{
			display(a);
			printf("\nPlayer1: %d",l1);
			printf("\nMachine: %d",l2);

			//player play
			if(plcheck(a) || l1 == 0) // checking stoping condition
			{ 
				printf("\n\nYou loss the match!!");
				break;
			}

			while(1)
			{
				printf("\n\nEnter index which you want to movie: ");
				int i,j;
				printf("\nEnter value of i: ");
				scanf("%d",&i);
				printf("Enter value of j: ");
				scanf("%d",&j);

				if (i<0 || j<0 || i>7 || j>7 || a[i][j] != 3) // invalid input
					printf("\nInValid input!");

				else if (i==0)
					printf("\nCant move!");
				
				else
				{
					if (j==0)
					{
						if (a[i-1][j+1] == 1)
						{
							plmoveR(a,i,j);
							break;
						}
						else if (i>1 && a[i-1][j+1] == 2 && a[i-2][j+2] == 1)
						{
							l2-=1;
							plmoveJR(a,i,j);
							break;
						}
						else
							printf("\nCant move!");
					}
					
					else if(j==7)
					{
						if (a[i-1][j-1] == 1)
						{
							plmoveL(a,i,j);
							break;
						}
							
						else if (i>1 && a[i-1][j-1] == 2 && a[i-2][j-2] == 1)
						{
							l2-=1;
							plmoveJL(a,i,j);
							break;
						}
						else
							printf("\nCant move!");
					}

					else
					{
						int x = 0,y = 0;
						if (a[i-1][j-1] == 1 || (i>1 && j>1 && a[i-1][j-1] == 2 && a[i-2][j-2] == 1))
							x = 1;
						if (a[i-1][j+1] == 1 || (i>1 && j<6 && a[i-1][j+1] == 2 && a[i-2][j+2] == 1))
							y = 1;
						
						if (x == 0 && y == 0)
						{
							printf("\nCant move!");
							continue;
						}
						char c='t';
						int	unt;

						if (x == 1 && y == 1)
						{
							printf("\n\nwhere you want to move R or L: ");
							fflush(stdin);
							scanf("%d",&unt)
							scanf("%c",&c);
							fflush(stdin);
						}
						
						if (c == 'l' || c == 'L' || (c == 't' && x == 1))
						{
							if (a[i-1][j-1] == 1)
								plmoveL(a,i,j);
							else
							{
								l2-=1;
								plmoveJL(a,i,j);
							}
							break;
						}
						
						if (c == 'r' || c == 'R' || (c == 't' && y == 1))
						{
							if (a[i-1][j+1] == 1)
								plmoveR(a,i,j);
							else
							{
								l2-=1;
								plmoveJR(a,i,j);
							}
							break;
						}
					}
				}
			}

			// Machine play
			printf("\n\nPlayer1: \n");
			display(a);
			
			if(mcheck(a) || l2 == 0)
			{
				printf("\n\n\nCongrulation you won the match!!\n");
				break;
			}
			int ai[N][N];
			
			#pragma omp parallel
					#pragma omp parallel for
					for (int i=0; i<N; i++)
					{
						for (int j=0; j<N; j++)
							ai[i][j] = 0;
					}
				
			int x=0;
			
			#pragma omp parallel
					#pragma omp parallel for
					for (int i=0; i<7; i++)
					{
							for (int j=0; j<N; j++)
							{
								if (a[i][j] == 2)
								{
									if (j==0)
										AIcheckR(a,ai,i,j);
									else if(j==7)
										AIcheckL(a,ai,i,j);
									else
									{
										AIcheckR(a,ai,i,j);
										x = ai[i][j];
										AIcheckL(a,ai,i,j);
										if (ai[i][j] < x)
												ai[i][j] = x;
									}
								}
							}
						}
					
			int t=0,y=0;
			x=0;

			#pragma omp parallel
					#pragma omp parallel for
					for (int i=0; i<7; i++)
					{
						for (int j=0; j<N; j++)
						{
							if (ai[i][j] > t)
							{
								t = ai[i][j];
								x=i;
								y=j;
							}
						}
					}

			if (ai[x][y] > 40 || ai[x][y]==21 || ai[x][y]==22)
			{
				if (ai[x][y]%10 == 1)
					moveJR(a,x,y);
				
				else
					moveJL(a,x,y);
				
				l1-=1;
			}
			else
			{
				if (ai[x][y]%10 == 1)
					moveR(a,x,y);
				else
					moveL(a,x,y);
			}
			printf("\nMachine:\n");
		}
}
