import os
import sys

if __name__ == "__main__":
    
    #os.chdir('E:\\CarData\\voc2007\\training\\car')
    fnm=sys.argv[1]
    #fnm='009959'
    fl=open('%s_objs.txt'%(fnm),'r')
    array=[]
    for line in fl:
        array.append( line.strip() )
        
    fls=array[1:int(array[0])+1]
    
    for fl in fls:
        ta=fl.split(' ')
        print ' '.join( x for x in ta)
    
    