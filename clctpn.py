import os
import sys
if __name__ == "__main__":
    os.chdir('E:\\CarData\\voc2007\\transfer')
    
    features=[]
    
    ins = open( "positive.lst", "r" )
    array = []
    for line in ins:
        array.append( line.strip() )
    fls=array[1:int(array[0])+1]
    
    ins.close();
    for fl in fls:             
        temfile=open(fl+'_feas.txt','r')
        for sth in temfile:
            features.append(sth.strip())       
        temfile.close()  
    
    ins = open( "negative.lst", "r" )
    array = []
    for line in ins:
        array.append( line.strip() )
    fls=array[1:int(array[0])+1]
    ins.close()
    
    for fl in fls:             
        temfile=open(fl+'_feas.txt','r')
        for sth in temfile:
            features.append(sth.strip())        
        temfile.close()  
    
        
        
    
    allf=open('allFeaturespn.txt','wb')
    for fea in features:
        allf.write('%s\n'%(fea))
    
    allf.close()
        