### Simple kernel module example

## Insert kernel module(the module needs to be built)
```
sudo insmod hello.ko
```

## Make kernel module device(character device in this case) file
```
sudo mknod [FILE] c [MAJOR] [MINOR]
```

## Change permisions for [FILE]
```
sudo chmod 666 [FILE]
```


## Remove kernel module
```
sudo rmmod hello 
```
