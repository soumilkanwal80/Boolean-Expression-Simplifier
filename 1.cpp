#include<iostream>
#include<fstream>
#include<map>
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
			cout<<s[0]<<s[1]<<"Maxterms are - ";
			for(int i=0;i<row;i++)
			{
				if(output[i] == 0)
					cout<<i<<" ";
			}
			cout<<endl;
		}
		cout<<endl;
		of<<s[1]<<" = m(";
		for(int i=0;i<row;i++)
		{
			
			if(output[i]&1)
			{
				of<<i<<" ";
			}
			

		}
		of<<")\n";
	}
    of.close();
	f.close();
	cout<<"Your minterms have been generated and exported to output.txt\n";

}
