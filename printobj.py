import os
import sys

if __name__ == "__main__":
    
    #os.chdir('E:\\CarData\\voc2007\\demo\\car')
    fln=sys.argv[1]
    #fln='000007'
    
    ins = open( '%s_objs.txt'%(fln), "r" )
    array = []
    for line in ins:
        array.append( line.strip() )
    
    fls=array[1:int(array[0])+1]
    
    for i in range(0,len(fls)):
        if(fls[i].split(' ')[4]=='0'):
            print '%s_obj_%d'%(fln,i)