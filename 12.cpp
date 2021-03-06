#include<iostream>
#include<fstream>
#include<algorithm>
#include<stack>
#include<list>
#include<cmath>
#include<vector>
using namespace std;

int check(char c)
{
	if(c>=97 && c<=122) // check for character
		return 1;
	else if((int)c == 39)	// check for complement
		return 2;

	else if(c == '(') 	// POS
		return 3;

	else if(c == ')')	// POS
		return 4;

	else if(c == '+') 	// OR
		return 5;

	else				// spaces etc
		return 0;
}

int numberof1(int m)
{
    int c = 0;
    while(m>0)
    {
        c += m&1;
        m>>=1;
    }
    return c;
}

struct Implicant
{
    int *array;
    int size;
    Implicant(int _size)
    {
        size = _size;
        array = new int[size];
    }
    
    Implicant(int _size, Implicant i1, Implicant i2)
    {
        size = _size;
        array = new int[size];
        for(int i=0;i<i1.size;i++)
        {
            array[i] = i1.array[i];
        }
        
        for(int i=0;i<i2.size;i++)
        {
            array[i+i2.size] = i2.array[i];
        }
        
        sort(array, array+size);
    }
};

struct Bucket
{
    list<Implicant> implicants;
};

struct BucketStore
{
    stack<Bucket**> store;
};

bool isSuperSet(Implicant super_set, Implicant sub_set)
{
    int super_set_size =super_set.size;
    int sub_set_size = sub_set.size;
    int count = 0;
    for(int i=0;i<super_set_size;i++)
    {
        for(int j=0;j<sub_set_size;j++)
        {
            if(super_set.array[i] == sub_set.array[j])
            {
                count++;
            }
        }
    }
    
    if(count == sub_set_size)
        return true;
    else
        return false;
}

int main()
{
	fstream f("inputs.txt", ios::in);
	fstream of("outputs.txt", ios::out);
	string s1, s2;
	getline(f, s1);
	getline(f, s2);
	int count = 0;
	for(int i = 6;i<s1.length();i++)
	{
		if(check(s1[i]) == 1)
			count++;
	}

		int row = 1<<count;
		int col = 2*count;
		bool truth_table[row][col];

		for(int i=0;i<row;i++)
		{
			int temp = i;
			int j = col - 2;
			while(j>=0)
			{
				truth_table[i][j] = temp%2;
				truth_table[i][j+1] = !truth_table[i][j];
				temp /= 2;
				j -= 2;
			}
		}

	string s;
	while(getline(f, s))
	{
		cout<<"Entered Expression is: "<<s<<endl;
		bool isPOS = 0;
		for(int i=0;i<s.length();i++)					
			if(s[i] == '(')
			{
				isPOS = 1;
				break;
			}
		// int mini = 123;
		// int maxi = 96;
		// for(int i=0;i<s.length();i++)							
		// {
		// 	if(s[i] >= 97 && s[i]<=122)
		// 	{
		// 		mini = min(mini, (int)s[i]);
		// 		maxi = max(maxi, (int)s[i]);
		// 	}
		// }
		// int count = maxi - mini + 1;


		bool output[row];
		for(int i=0;i<row;i++)
			output[i] = 0;

		if(isPOS == 0)
		{
			for(int j=0;j<row;j++)
			{
				bool temp1 = 1, flag = 0;
				for(int i = 2;i<s.length();i++)
				{
					if(check(s[i]) == 1)
					{
						int val = count - s[i] + 97 - 1;
						val *= 2;
						if(check(s[i+1]) == 2)
						{
							val++;
							i++;
						}
						temp1 = temp1&truth_table[j][val];
					}	

					else if(check(s[i]) == 5 && flag == 0)
					{
						output[j] = temp1;
						temp1 = 1;
						flag = 1;
					}	

					else if(check(s[i]) == 5 && flag == 1)
					{
						output[j] = output[j] | temp1;
						temp1 = 1;
					}
				}
				output[j] = output[j] | temp1;
			}	

			cout<<s[0]<<s[1]<<" Minterms are - ";
			for(int i=0;i<row;i++)
			{
				if(output[i]&1)
					cout<<i<<" ";
			}
			cout<<endl;
		}

		else 
		{
			for(int j =0 ;j<row;j++)
			{
				bool temp1 = 0, flag = 0;
				for(int i=2;i<s.length();i++)
				{
					if(check(s[i]) == 3)			//'('
						continue;

					else if(check(s[i]) == 1)		// a-z
					{	
						int val = count - s[i] + 97 - 1;
						val *= 2;
						if(check(s[i+1]) == 2)		//'
						{
							val++;
							i++;
						}
						temp1 = temp1|truth_table[j][val];
					}

					else if(check(s[i]) == 4 && flag == 0)
					{
						output[j] = temp1;
						flag = 1;
						temp1 = 0;
					}

					else if(check(s[i]) == 4 && flag == 1)
					{
						output[j] = output[j]&temp1;
						temp1 = 0;
					}
				}
			}
			cout<<s[0]<<s[1]<<" Maxterms are - ";
			for(int i=0;i<row;i++)
			{
				if(output[i] == 0)
					cout<<i<<" ";
			}
			cout<<endl;
		}
		// cout<<endl;
		int n = 0;

		of<<s[1]<<" = m(";
		for(int i=0;i<row;i++)
		{
			
			if(output[i]&1)
			{

				of<<i<<" ";
				n++;
			}
			

		}
		of<<")\n";
        // cout<<"Number of terms is: "<<n<<endl;
        int a[n];
        int iter = 0;
        int a_copy[n];
        for(int i=0;i<row;i++)
        {
        	if(output[i]&1)
        	{
        		a_copy[iter] = i;
        		a[iter] = i;
        		iter++;
        	}
        }
        if(n == 1 && a[n-1] == 0)
        {
            cout<<"Reduced Expression is: a'"<<endl;
            continue;
        }
        sort(a, a+n);
        int size_of_bucket = ceil(log2(a[n-1])) + 2;
        
        Bucket **pBucket = new Bucket*[size_of_bucket];
        for(int i=0;i<size_of_bucket;i++)
            pBucket[i] = new Bucket;
        
        for(int i=0;i<n;i++)
        {
            Implicant e(1);
            e.array[0] = a[i];
            pBucket[numberof1(a[i])]->implicants.push_back(e);
        }
        BucketStore s_store;
        while(1)
        {
            bool flag = 0;
            s_store.store.push(pBucket);
            Bucket **nBucket = new Bucket* [size_of_bucket];
            for(int i=0;i<size_of_bucket;i++)
                nBucket[i] = new Bucket;
            
            for(int i=1;i<size_of_bucket;i++)
            {
                list<Implicant> ::iterator it1, it2;
                for(it1 = pBucket[i-1]->implicants.begin();it1!=pBucket[i-1]->implicants.end();it1++)
                {
                    for(it2 = pBucket[i]->implicants.begin();it2!=pBucket[i]->implicants.end();it2++)
                    {
                        
                        bool check = 0;
                        for(int j=0;j<it1->size;j++)                // Getting r1
                        {
                            int temp = it2->array[j] - it1->array[j];
                            if(ceil(log2(temp))!=floor(log2(temp)))
                            {
                                check = 1;
                                break;
                            }
                        }
                        
                        if(check == 0)
                        {
                            Implicant e(2 * it1->size, *it1, *it2);
                            bool unique = 1;
                            for(int k=1;k<e.size;k++)
                            {
                                if(e.array[k-1] == e.array[k])
                                {
                                    unique = 0;
                                    break;
                                }
                            }
                            bool unique2 = 1;
                            if(nBucket[i-1]->implicants.size() != 0)
                            {
                                list<Implicant> :: iterator it3;
                                for(it3 = nBucket[i-1]->implicants.begin();it3!=nBucket[i-1]->implicants.end();it3++)
                                {
                                    int count2 = 0;
                                    for(int k=0;k<e.size;k++)
                                    {
                                        if(it3->array[k] == e.array[k])
                                            count2++;
                                        
                                    }
                                    if(count2 == e.size)
                                    {
                                        unique2 = 0;
                                        break;
                                    }
                                }
                                
                            }
                            if(unique == 1 && unique2 == 1)
                                nBucket[i-1]->implicants.push_back(e);
                            flag = 1;
                        }
                    }
                }
                
            }
            pBucket = new Bucket*[size_of_bucket];
            for(int i=0;i<size_of_bucket;i++)
            {
                pBucket[i] = new Bucket;
                pBucket[i]->implicants = nBucket[i]->implicants;
            }
            
            // for(int i=0;i<size_of_bucket-1;i++)
            // {
            //     list<Implicant> :: iterator it;
            //     for(auto it = pBucket[i]->implicants.begin();it!=pBucket[i]->implicants.end();it++)
            //     {
            //         cout<<i<<": ";
            //         for(int j=0;j<it->size;j++)
            //             cout<<it->array[j]<<" ";
            //         cout<<endl;
            //     }
            //     cout<<endl;
            //  }
            // size_of_bucket--
            if(flag == 0)
                break;
            
        }
        // Step B2
        Bucket prime_implicants;
        while(1)
        {
            bool check = 1;
            
            Bucket **temp = s_store.store.top();
            s_store.store.pop();
            for(int i=0;i<size_of_bucket;i++)
            {
                if(temp[i]->implicants.size()!=0)
                {
                    check = 0;
                    s_store.store.push(temp);
                    break;
                }
            }
            if(check == 0)
                break;
            
        }
        
        
        
        Bucket ** temp = s_store.store.top();
        s_store.store.pop();
        for(int i=0;i<size_of_bucket;i++)
        {
            list<Implicant> :: iterator it;
            for(it = temp[i]->implicants.begin();it!=temp[i]->implicants.end();it++)
            {
                prime_implicants.implicants.push_back(*it);
            }
        }
        
        
        
        while(s_store.store.size()>0)
        {
            temp = s_store.store.top();
            s_store.store.pop();
            list<Implicant> :: iterator it1, it2;
            
            for(int i=0;i<size_of_bucket;i++)
            {
                for(it1 = temp[i]->implicants.begin();it1!=temp[i]->implicants.end();it1++)
                {
                    bool check = 0;
                    
                    for(it2 = prime_implicants.implicants.begin();it2!=prime_implicants.implicants.end();it2++)
                    {
                        
                        if(isSuperSet(*it2, *it1) == true)
                        {
                            check = 1;
                            break;
                        }
                    }
                    
                    if(check == 0)
                    {
                        prime_implicants.implicants.push_back(*it1);
                        
                    }
                }
            }
            
        }
    
        // for(auto it = prime_implicants.implicants.begin();it!=prime_implicants.implicants.end();it++)
        // {
        //     for(int j=0;j<it->size;j++)
        //     {
        //         cout<<it->array[j]<<" ";
        //     }
        
        //     cout<<endl;
        // }
        
        // Step B3
        Bucket ess_prime_implicants;
        
        list<Implicant> :: iterator it, it2;
            
        // Step 1
        for(int i=0;i<n;i++)
        {
            int count = 0;  
            bool pk = 1;         
            for(it = prime_implicants.implicants.begin();it!=prime_implicants.implicants.end();it++)
            {
                for(int j =0;j<it->size;j++)
                {
                    if(it->array[j] == a[i])
                    {
                        count++;
                        it2 = it;
                    }  
                    if(count>1)
                    {
                        pk = 0;
                        break;
                    }       
                }
            }
            if(pk == 1 && count == 1)
            {
                ess_prime_implicants.implicants.push_back(*it2);
            }
        }


        while(1)
        {
            // 1a
            for(it = ess_prime_implicants.implicants.begin();it!=ess_prime_implicants.implicants.end();it++)
            {
                for(int j=0;j<it->size;j++)
                {
                    int tb_rmvd = it->array[j];
                    for(int k=0;k<n;k++)
                    {
                        if(tb_rmvd == a[k])
                            a[k] = -1;
                    }
                }
            }

            //Step 2
            vector<list<Implicant> :: iterator> vit;
            for(it = prime_implicants.implicants.begin();it!=prime_implicants.implicants.end();it++)
            {
                int found = 0;
                for(int j = 0;j<it->size;j++)
                {
                    for(int i=0;i<n;i++)
                    {
                        if(it->array[j] == a[i])
                        {
                            found++;
                            break;
                        }
                    }
                    if(found>0)
                        break;
                }
                
                if(found == 0)
                    vit.push_back(it);
            }

            for(int i=0;i<vit.size();i++)
                prime_implicants.implicants.erase(vit[i]);
            // Step 3
            list<Implicant> :: iterator val;
            int count = -1;
            bool fnd = 0;
            if(prime_implicants.implicants.size()>0)
            {
                for(it = prime_implicants.implicants.begin();it!=prime_implicants.implicants.end();it++)
                {
                    int cnt = 0;
                    for(int i=0;i<n;i++)
                    {
                        for(int j = 0;j<it->size;j++)
                        {
                            if(it->array[j] == a[i])
                            {
                                cnt++;
                            }
                        }
                    }
                    
                    if(cnt>count)
                    {
                        count = cnt;
                        val = it;
                        fnd = 1;
                    }
                }
                if(fnd == 1)
                    ess_prime_implicants.implicants.push_back(*val);
                
                for(int j=0;j<val->size;j++)
                {
                    for(int i=0;i<n;i++)
                        if(a[i] == val->array[j])
                        {
                            a[i] = -1;
                            break;
                        }
                }
            }
            else
                break;
            // Loop Termination
            int cnt = 0;
            for(int i=0;i<n;i++)
            {
                if(a[i]!=-1)
                {
                    cnt++;
                }
            }
            if(cnt == 0)
                break;
        }
        
        list<Implicant> :: iterator it1, it3;
        for(it1 = ess_prime_implicants.implicants.begin();it1!=ess_prime_implicants.implicants.end();it1++)
        {
            bool erase = 0;
            for(it2 = ess_prime_implicants.implicants.begin();it2 != ess_prime_implicants.implicants.end();it2++)
            {
                if(it1!=it2 && it1->size == it2->size)
                {
                    int ccheck = 0;
                    for(int i = 0;i<it1->size;i++)
                    {
                        if(it1->array[i] == it2->array[i])
                            ccheck++;
                    }
                    
                    if(ccheck == it1->size)
                    {
                        it3 = it2;
                        erase = 1;
                        break;
                    }
                }
            }
            if(erase == 1)
                ess_prime_implicants.implicants.erase(it3);
        }
        
        // for(auto it = ess_prime_implicants.implicants.begin();it!=ess_prime_implicants.implicants.end();it++)
        // {
        //     for(int j=0;j<it->size;j++)
        //     {
        //         cout<<it->array[j]<<" ";
        //     }
            
        //     cout<<endl;
        // }
        
        // Step B4
        bool ch23 = 0;
        cout<<"Reduced Expression is: ";
        for(it1 = ess_prime_implicants.implicants.begin();it1!=ess_prime_implicants.implicants.end();it1++)
        {
            int rw = it1->size;
            int col = ceil(log2(a_copy[n-1]));
            if(ceil(log2(a_copy[n-1])) == floor(log2(a_copy[n-1])))
                col++;
            int terms[rw][col];
            for(int i=0;i<rw;i++)
            {
                for(int j=0;j<col;j++)
                    terms[i][j] = 0;
            }
            
            for(int i=0;i<it1->size;i++)
            {
                int num = it1->array[i];
                int j = col - 1;
                while(num!=0)
                {
                    terms[i][j] = num%2;
                    num/=2;
                    j--;
                }
            }
            
            int ans[col];
            for(int j = 0;j<col;j++)
            {
                bool zero_check = 0, one_check = 0;
                for(int i=0;i<rw;i++)
                {
                    if(terms[i][j] == 0)
                        zero_check = 1;
                    else
                        one_check = 1;
                    
                }
                
                if(zero_check == 1 && one_check == 1)
                    ans[j] = -1;
                else if(zero_check == 0 && one_check == 1)
                    ans[j] = 1;
                else if(zero_check == 1 && one_check == 0)
                    ans[j] = 0;
            }

            bool flag = 0;
            for(int i = 0;i<col;i++)
            {
                char c = col - i - 1 + 97;
                if(ans[i] == 0)
                {
                    flag = 1;
                    ch23 = 1;
                    cout<<c<<"'";
                }
                else if(ans[i] == 1)
                {
                    flag = 1;
                    cout<<c;
                    ch23 = 1;
                }
            }
            it2 = it1;
            it2++;
            if(flag == 1 && (it2) != ess_prime_implicants.implicants.end())
                cout<<" + ";
        }
        if(ch23 == 0)
            cout<<"1";
        cout<<endl<<endl;
         
	}
    of.close();
    f.close();
    cout<<"Your Minterms have been exported to outputs.txt file\n";

}
