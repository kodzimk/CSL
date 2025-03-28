<div align="center">
<pre>
CSL(C Styled Language)
---------------------------------------------------
Lanuage written in c++ with decent percent of syntax as in c
</pre>
</div>

# Installation andd running 

install this repo(Min version of c++ 20).

```sh
git clone (link of project)
cmake -B ./build
run.sh
```

# Examples

```sh
int main()
{
  int i = 10;
  int counter = 0;
  int arr[10]; 
  cin(arr[0]);

  while(i > 0)
  {
  arr[counter] = i;
  i--;
  counter++;
  }

 while(10 > i)
 {
    if(arr[i] % 2 == 0)
      {
        printf(arr[i]);
      }
      else if(arr[i] % 2 == 1)
      {
        printf("Niger: ");
        printf(arr[i]);
      }
    i++;
 } 
  return 1;
}
```
# Requirements
* Linux(any type)(suggest Ubuntu latest version)<br />
* nasm
