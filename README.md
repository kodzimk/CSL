<div align="center">
<pre>
CSL(C Styled Language)
---------------------------------------------------
Lanuage written in c++ with decent percent of syntax as in c
</pre>
</div>

# Installation andd running 

install this repo(Min version of c++ 20).

Build<br>
```sh
git clone (link of project)
cmake -B ./build
```

Run<br>
```sh
"add main.csl file in build folder and run project then" run this scirpt above
./run.sh
```


# Examples

```sh
int main()
{
  int i = 0;
  int arr[10];
  while(10 > i)
  {
    arr[i] = i;
    printf("Array value: ");
    printf(arr[i);
    i++;
  }

  return 1;
}
```
# Requirements
* cmake
* Linux(any type)(suggest Ubuntu latest version)<br />
* nasm
