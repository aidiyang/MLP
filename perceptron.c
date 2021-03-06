#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
/*
MULTILAYER PERCEPTRON!
FORMULA TO BE USED IN ERROR BETWEEN HIDDEN NODE AND OUTPUT LAYER:
E`(wrt node k in hidden layer)=-(desired_output_k-current_output_k)*f`(sum_of_weights_with_hiddennode_values)
FORMULA TO BE USED IN ERROR BETWEEN HIDDEN NODE AND OUTPUT LAYER:
E`(wrt node j in input layer)=sum(-error_in_hidden_layer*weight_of_kj*f`(sum_of_weights_with_hidden_node_values)*input_node_i)
3D weight matrix
1st Dimension - Layer
2nd & 3rd Dimension - locating the weight
*/
FILE *train,*test;
float *maxtest,*maxtrain,*mintest,*mintrain;
float **prevhiddenweights;
float **previnputweights;
float ***weights;
float **data;
float *hiddendata;
float **testdata;
int Number_neurons[3]={13,-1,3};
float *errorhidden;
float *errorinput;
float output[3];
float acc[10];
float ep[10];
float no[10];
int count;
float sigmoidfun(float x)
{
	return (float)((1.0)/(1+exp(-1*x)));
}
float derivativefun(float x)
{
	return (float)(sigmoidfun(x)*(1-sigmoidfun(x)));
}
float randomweights()
{
	return ((float)(rand()) / (float)(RAND_MAX)) * (0.5+0.5) -0.5;
}
void loaddata()
{
	train=fopen("Wine data/train.csv","r");
	int i=0,j=0;
	data=new float*[119];
	for(;i<118;i++)
		data[i]=new float[15];
	i=0;
	while(fscanf(train,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",&data[i][1],&data[i][2],&data[i][3],&data[i][4],&data[i][5],&data[i][6],&data[i][7],&data[i][8],&data[i][9],&data[i][10],&data[i][11],&data[i][12],&data[i][13],&data[i][14])!=EOF)
	{
		//printf("%d\n",i);
		data[i][0]=1.0;
		//printf("%f\n",data[i][14]);
		i++;
	}
	fclose(train);
	test=fopen("Wine data/test.csv","r");
	i=0;
	testdata=new float*[61];
	for(;i<118;i++)
		testdata[i]=new float[15];
	i=0;
	while(fscanf(test,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",&testdata[i][1],&testdata[i][2],&testdata[i][3],&testdata[i][4],&testdata[i][5],&testdata[i][6],&testdata[i][7],&testdata[i][8],&testdata[i][9],&testdata[i][10],&testdata[i][11],&testdata[i][12],&testdata[i][13],&testdata[i][14])!=EOF)
	{
		//printf("%d\n",i);
		testdata[i][0]=1.0;
		i++;
	}
	//getchar();
	fclose(test);
	mintest=new float[13];
	mintrain=new float[13];
	maxtest=new float[13];
	maxtrain=new float[13];
	for(j=1;j<14;j++)
    {
        maxtrain[j-1]=mintrain[j-1]=data[0][j];
        maxtest[j-1]=mintest[j-1]=testdata[0][j];

                for(i=0;i<118;i++)
                {
                        if(maxtrain[j-1]<data[i][j])
                        maxtrain[j-1]=data[i][j];
                        if(mintrain[j-1]>data[i][j])
                        mintrain[j-1]=data[i][j];
                        if(i<60)
                        {
                            if(maxtest[j-1]<testdata[i][j])
                            maxtest[j-1]=testdata[i][j];
                            if(mintest[j-1]>testdata[i][j])
                            mintest[j-1]=testdata[i][j];
                        }
                }

	}
    for(i=0;i<118;i++)
    {
        for(j=0;j<13;j++)
        {
            data[i][j+1]=(data[i][j+1]-mintrain[j])/(maxtrain[j]-mintrain[j]);
            if(i<60)
                testdata[i][j+1]=(testdata[i][j+1]-mintest[j])/(maxtest[j]-mintest[j]);
        }
    }
}
void init(int n)
{
	Number_neurons[1]=n;
	weights=new float**[2];
	int i=0,j=0,k=0;
	for(;i<2;i++)
	{
		if(!i)
		{
			weights[i]=new float*[Number_neurons[0]+1];
			for(j=0;j<Number_neurons[0]+1;j++)
			{
				weights[i][j]=new float[Number_neurons[1]];
				for(k=0;k<Number_neurons[1];k++)
				{
					weights[i][j][k]=randomweights();
				//	printf("%d %d %d %f\n",i,j,k,weights[i][j][k]);
				}
			}
		}
		else
		{
			weights[i]=new float*[Number_neurons[1]+1];
			for(j=0;j<Number_neurons[1]+1;j++)
			{
				weights[i][j]=new float[Number_neurons[2]];
				for(k=0;k<Number_neurons[2];k++)
				{
					weights[i][j][k]=randomweights();
			//		printf("%d %d %d %f\n",i,j,k,weights[i][j][k]);
				}
			}
		}

	}

}
void run_model()
{
	int i=0,j=0,k=0,datavar=0;float sum;
	hiddendata=new float[Number_neurons[1]+1];
		float *netk=new float[3];
			float *exp=new float[3];
	/* Calculation of value in hidden layer */
	float *netj=new float[Number_neurons[1]];
	label1:/*
	for(i=0;i<15;i++)
	{
		printf("%f ",data[datavar][i]);
	}
		printf("\n");*/
	for(i=0;i<Number_neurons[1];i++)
	{
		sum=0.0;
		for(k=0;k<14;k++)
		{
			sum+=weights[0][k][i]*data[datavar][k];
		}
		netj[i]=sum;
		hiddendata[i+1]=sigmoidfun(sum);
	}
	hiddendata[0]=1;//bias
	/*Calculate the 1st output*/
	//printf("Values for input %d 1st epoch\n",datavar+1);
	for(i=0;i<3;i++)
	{
		sum=0.0;
		for(k=0;k<Number_neurons[1]+1;k++)
		{
			sum+=weights[1][k][i]*hiddendata[k];
		}
		netk[i]=sum;
		output[i]=sigmoidfun(sum);
		//printf("%f ",output[i]);

	}
	printf("\n");
	/*
		hardcoding first backpropogation
	*/

	switch((int)data[datavar][14])
	{
		case 1:
			exp[0]=1.0;
			exp[1]=0.0;
			exp[2]=0.0;
			break;
		case 2:
			exp[0]=0.0;
			exp[1]=1.0;
			exp[2]=0.0;
			break;
		case 3:
			exp[0]=0.0;
			exp[1]=0.0;
			exp[2]=1.0;
	}
	/* hidden layer and output layer updation*/

//printf("Weights between output layer and hidden layer:\n");
	int step=0;
float error=0;
float *dk=new float[3];
float *dj=new float[Number_neurons[1]];
do
	{


		for(i=0;i<3;i++)
		{
            error+=(exp[i]-output[i])*(exp[i]-output[i]);
			dk[i]=(exp[i]-output[i])*derivativefun(netk[i]);
			//for(j=0;j<Number_neurons[1]+1;j++)
			//{
				//weights[1][j][i]-=0.01*-1*dk[i]*hiddendata[j];
			//}
		}

		/* between hidden layer and input*/

			for(j=0;j<Number_neurons[1];j++)
			{
				sum=0.0;
				for(k=0;k<Number_neurons[2];k++)
				{
					sum+=dk[k]*weights[1][j][k]*derivativefun(netj[j]);
				}
				//weights[0][i][j]-=0.01*sum*data[datavar][i];
				dj[j]=sum;
			}


		for(i=0;i<Number_neurons[0]+1;i++)
		{
				for(j=0;j<Number_neurons[1];j++)
				{
					weights[0][i][j]+=0.1*dj[j]*data[datavar][i];
				}
		}

		for(i=0;i<Number_neurons[1]+1;i++)
		{
				for(j=0;j<Number_neurons[2];j++)
				{
					weights[1][i][j]+=0.1*dk[j]*hiddendata[i];
				}
		}





		datavar++;
		if(datavar>117)
			{		step++;
			datavar=datavar%117;
			//printf("%f\n",error);
			if(error<0.01)
            break;
			error=0;
			/*for(i=1;i<Number_neurons[1]+1;i++)
			{
				printf("%d %f\n",i,hiddendata[i]);
			}*/
			}

		/* After first backpropogation*/
		for(i=0;i<Number_neurons[1];i++)
		{
			sum=0.0;
			for(k=0;k<14;k++)
			{
				sum+=weights[0][k][i]*data[datavar][k];
			}
			netj[i]=sum;
			hiddendata[i+1]=sigmoidfun(sum);
		}
	//	printf("Values for input backpropogation\n");
		for(i=0;i<3;i++)
		{
			sum=0.0;
			for(k=0;k<Number_neurons[1]+1;k++)
			{
				sum+=weights[1][k][i]*hiddendata[k];
			}
			netk[i]=sum;
			output[i]=sigmoidfun(sum);
			//printf("%f ",output[i]);

		}
		//printf("\n");
		switch((int)data[datavar][14])
		{
        case 0:
                printf("WTF!?");
                getchar();
                break;
			case 1:
				exp[0]=1.0;
				exp[1]=0.0;
				exp[2]=0.0;
				break;
			case 2:
				exp[0]=0.0;
				exp[1]=1.0;
				exp[2]=0.0;
				break;
			case 3:
				exp[0]=0.0;
				exp[1]=0.0;
				exp[2]=1.0;
		}
		//printf("\n\n\n");
		//datavar++;

	}while(1);
	//printf("%f %f %f\n",output[0],output[1],output[2]);
	printf("Done! epochs:%d\n",step);
	/*for(i=0;i<Number_neurons[0]+1;i++)
	{
		for(j=0;j<Number_neurons[1];j++)
		{
								printf("%d %d %d %f\n",0,i,j,weights[0][i][j]);
		}
	}
	for(i=0;i<Number_neurons[1]+1;i++)
	{
		for(j=0;j<Number_neurons[2];j++)
		{
								printf("%d %d %d %f\n",1,i,j,weights[1][i][j]);
		}
	}*/
	datavar=0;
	float cor=0.0;
	while(datavar<118){
	//scanf("%d",&datavar);
	for(i=0;i<Number_neurons[1];i++)
		{
			sum=0.0;
			for(k=0;k<14;k++)
			{
				sum+=weights[0][k][i]*data[datavar][k];
			}
			netj[i]=sum;
			hiddendata[i+1]=sigmoidfun(sum);
		}
	//	printf("Values for input backpropogation\n");
		for(i=0;i<3;i++)
		{
			sum=0.0;
			for(k=0;k<Number_neurons[1]+1;k++)
			{
				sum+=weights[1][k][i]*hiddendata[k];
			}
			netk[i]=sum;
			output[i]=sigmoidfun(sum);
			//printf("%f ",output[i]);

		}
		float cl=-1;
		if(output[0]>output[1] && output[0]>output[2])
        {
            cl=1.0;
        }
        if(output[1]>output[0] && output[1]>output[2])
        {
            cl=2.0;
        }
        if(output[2]>output[0] && output[2]>output[1])
        {
            cl=3.0;
        }

        if(cl==data[datavar][14])
            cor++;
         else
        {
            printf("Entry: %d output: %d actual class: %d\n",datavar+1,cl,data[datavar][14]);
        }
		datavar++;
		}
		printf("%f %% accurate on train data\n",(cor/118.0)*100);
        datavar=0;
	cor=0.0;
	while(datavar<60){
	//scanf("%d",&datavar);
	for(i=0;i<Number_neurons[1];i++)
		{
			sum=0.0;
			for(k=0;k<14;k++)
			{
				sum+=weights[0][k][i]*testdata[datavar][k];
			}
			netj[i]=sum;
			hiddendata[i+1]=sigmoidfun(sum);
		}
	//	printf("Values for input backpropogation\n");
		for(i=0;i<3;i++)
		{
			sum=0.0;
			for(k=0;k<Number_neurons[1]+1;k++)
			{
				sum+=weights[1][k][i]*hiddendata[k];
			}
			netk[i]=sum;
			output[i]=sigmoidfun(sum);
			//printf("%f ",output[i]);

		}
        float cl=-1;
		if(output[0]>output[1] && output[0]>output[2])
        {
            cl=1.0;
        }
        if(output[1]>output[0] && output[1]>output[2])
        {
            cl=2.0;
        }
        if(output[2]>output[0] && output[2]>output[1])
        {
            cl=3.0;
        }
        if(cl==testdata[datavar][14])
            cor++;
        else
        {
            printf("Entry: %d output: %f actual class: %f\n",datavar+1,cl,testdata[datavar][14]);
        }
		datavar++;
		}
        printf("%f %% accurate on test data\n",(cor/60.0)*100);
        ep[count]=step;
        acc[count]=(cor/60.0)*100;
        no[count]=Number_neurons[1];

	//if(datavar<3)
		//goto label1;
/*for(i=0;i<Number_neurons[1]+1;i++)
    delete hiddendata;*/
}/*
void backpropogation()
{

}*/
int main()
{
	srand((unsigned int)time(NULL));
	loaddata();
	int j=10;
	count=0;
    for(;j<21;j++)
	{
    printf("Number of Nodes: %d\n",j);
    init(j);
	run_model();
    count++;
	}
	for(j=0;j<count;j++)
    {
        printf("%f,%f,%f\n",ep[j],no[j],acc[j]);
    }
	return 0;
}
