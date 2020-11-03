#include<iostream>
#include<vector>
#include<cstring>
using namespace std;
struct FCB{
    char file_name[8];
    char extension[3];
    long size;
    int start;
}temp;
vector<FCB> fcb_vec;//declaring vector of FCB for dynamic work
class info{
    private:
        int *FAT,block_no;
        long block_size;
    public:
        void declare(int n){
            FAT = new int [n];
            for(int i=0;i<n;i++){
                FAT[i]=0;
            }
            block_no=n;
        }
        void get_val(long n){ 
            block_size=n;
        }
        void allocate(int size){
            if(size<=block_size){
                for(int i=0;i<block_no;i++){
                    if(FAT[i]==0){
                        temp.start=i;
                        FAT[i]=-1;
                        break;
                    }
                }
            }
            else if(size>block_size&&size%block_size==0){
                int x=size/block_size,c=0,next;
                for(int i=0;i<this->block_no;i++){
                    if(x>0&&FAT[i]==0){
                        if(c==0){
                            temp.start=i;
                            next=i;
                            c++;
                        }
                        else{
                            FAT[next]=i;
                            FAT[i]=-1;
                            next=i;
                        }
                        x--;
                    }
                }
            }
            else{
                int x=size/block_size,c=0,next;

                for(int i=0;i<block_no;i++){
                    if(x>=0&&FAT[i]==0){
                        if(c==0){
                            temp.start=i;
                            next=i;
                            c++;
                        }
                        else{
                            FAT[next]=i;
                            FAT[i]=-1;
                            next=i;
                        }
                        x--;
                    }
                }
            }
        }
        void showFAT(struct FCB* temp_s){
            int i=temp_s->start,next=FAT[temp_s->start];

            cout<<"\nFAT of File\n|\tCurrent\t\t|\tNext\t|"<<endl;

            do{
                next=FAT[i];
                cout<<"|\t"<<i<<"\t\t|\t"<<next<<"\t|"<<endl;
                i=FAT[i];
            }while(next!=-1);

        }
        void Delete(int index){
            char choice;
            cout<<"Press Y to if you delete complete file:";
            cin>>choice;
            if(choice=='y'||choice=='Y'){
            	int i=fcb_vec[index].start,next=fcb_vec[index].start;
	            do{
	                next=FAT[i];
	                FAT[i]=0;
	                i=next;
	            }while(next!=-1);
            	fcb_vec.erase(fcb_vec.begin()+index);
			}
			else{
				int block_no,no_of_block;
				cout<<"Enter block no from where you want to start deleting:";
                cin>>block_no;
                cout<<"Enter no. of blocks you want to delete:";
                cin>>no_of_block;
                int i=fcb_vec[index].start,next=fcb_vec[index].start,j,k;
                int *a = new int[no_of_block];
	            if(block_no==fcb_vec[index].start){
                        int n=block_no,m;
                    for(int x=0;x<no_of_block;x++){
                        m=FAT[n];
                        FAT[n]=0;
                        n=m;
                    }
                    fcb_vec[index].start=m;
	            }
	            else{
                    do{
	                next=FAT[i];
	                if(FAT[i]==block_no){
                        j=i;
	                }
	                i=next;
                    }while(next!=-1);
                    int n=block_no;
                    for(int x=0;x<no_of_block;x++){
                            a[x]=n;
                            n=FAT[n];
                    }
                    for(int x=0;x<no_of_block;x++){
                            if(x==no_of_block-1){
                                k=FAT[a[x]];
                            }
                            FAT[a[x]]=0;
                    }
                    FAT[j]=k;
                }
                fcb_vec[index].size-=(block_size*no_of_block);
            }
        }
        int free_block(){
        	int count=0;
        	for(int i=0;i<block_no;i++){
        		if(FAT[i]==0){
        			count++;
				}
			}
			return count;
		}
		void insert(int index){
		    int n;
		    cout<<"Enter no. of blocks you want to insert:";
		    cin>>n;
		    if(n<=free_block()){
                int x,y;
                cout<<"Enter block no. from which you want to insert:";
                cin>>x;
                int *arr = new int [n];
                int count = 0;
                for(int i = 0;i < block_no;i++){
                    if(FAT[i] == 0&&count<n){
                        arr[count] = i;
                        count++;
                    }
                }
                for(count=0;count<n-1;count++){
                    FAT[arr[count]]=arr[count+1];
                }
                if(x == fcb_vec[index].start){

                    fcb_vec[index].start = arr[0];
                    FAT[arr[n-1]] = x;
                }
                else if(FAT[x]==-1){

                    FAT[x]=arr[0];
                    FAT[arr[n-1]] = -1;
                }
                else{
                    for(int i = 0;i < block_no;i++){
                        if(FAT[i]==x){
                            y=i;
                            break;
                        }
                    }
                    FAT[y]=arr[0];
                    FAT[arr[n-1]] = x;
                }
                fcb_vec[index].size+=(block_size*n);
		    }
		    else{
                cout<<"Not Enough free blocks available"<<endl;
		    }
		}
};
info a;

class Menu{
    private:
        char choice;
    public:
        char Choices(){
            cout<<"\n Menu-"<<endl;
            cout<<"|     Enter your choice           |"<<endl;
            cout<<"|  1.Wanna open an existing file  |"<<endl;
            cout<<"|  2.Wanna create a new file      |"<<endl;
            cout<<"|  3.Wanna delete a block in file |"<<endl;
            cout<<"|  4.Insert a block in file       |"<<endl;
            cout<<"|  0.Exit or quit                 |"<<endl;
            cout<<endl;
            cin>>choice;
            return choice;
        }

};


void search(char* name){
    int condition=0;
    if(fcb_vec.size()==0){
        cout<<"No file stored"<<endl;
    }
    else{
        for (size_t i=0;i<fcb_vec.size();i++){
            int res = strcmp(fcb_vec[i].file_name,name);
            if(res==0){
                cout<<"|	FCB			|"<<endl;
                cout<<"|	Filename 	|"<<fcb_vec[i].file_name<<"\t|"<<endl;
                cout<<"|	Extension	|"<<fcb_vec[i].extension<<"\t|"<<endl;
                cout<<"|	File size	|"<<fcb_vec[i].size<<" byte\t|"<<endl;
                cout<<"|	start	 	|"<<fcb_vec[i].start<<"\t|"<<endl;

                a.showFAT(&fcb_vec[i]);
                condition=1;
                break;
            }
        }
        if(condition==0){
            cout<<"No such file found"<<endl;
        }
    }
}
void research(int *x,char* name){
    int condition=0;
    if(fcb_vec.size()==0){
        cout<<"No file stored"<<endl;
    }
    else{
        for (size_t i=0;i<fcb_vec.size();i++){
            int res = strcmp(fcb_vec[i].file_name,name);
            if(res==0){
                    *x=i;
                condition=1;
            }
        }
        if(condition==0){
            cout<<"No such file found"<<endl;
        }
    }
}
int main()
{
    long n,t;
    int index;
    cout<<"Enter no. of blocks threre are in memory:";
    cin>>n;
    a.declare(n);
    cout<<"Enter how much data each bock holds in bytes:";
    cin>>t;
    a.get_val(t);
    Menu menu_obj;
    char choice,name[8];
    do{
        choice=menu_obj.Choices();
        switch(choice){
            case '1'://search & open file
            	cout<<"Opening a file"<<endl;
                cout<<"Enter a filename:";
                cin>>name;
                search(name);
                cout<<"Remaining free blocks:"<<a.free_block()<<endl;
                break;
            case '2'://create a file
            	cout<<"Creating a file"<<endl;
                cout<<"Enter file name:";
                cin>>temp.file_name;
                cout<<"Enter file extension:";
                cin>>temp.extension;
                cout<<"Enter file size in bytes:";
                cin>>temp.size;
                a.allocate(temp.size);
                a.showFAT(&temp);
                fcb_vec.push_back(temp);
                cout<<"Remaining free blocks:"<<a.free_block()<<endl;
                break;
            case '3':
                cout<<"Enter a filename to edit:";
                cin>>name;
                research(&index,name);
                a.Delete(index);
                cout<<"Remaining free blocks:"<<a.free_block()<<endl;
                break;
            case '4':
                cout<<"Enter a filename to edit:";
                cin>>name;
                research(&index,name);
                a.insert(index);
		cout<<"Remaining free blocks:"<<a.free_block()<<endl;
                break;
            default:
                if(choice!='0')
                cout<<"Invalid choice..."<<endl;
        }
    }while(choice!='0');
}

