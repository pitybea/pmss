

from __future__ import print_function
import os
import sys


def loadfea(lg):
    ins = open( "%s.lst"%(lg), "r" )
    array = []
    for line in ins:
        array.append( line.strip() )
    
    fls=array[1:int(array[0])+1]
    features=[]
    positions=[]
    for fl in fls:             
        temfile=open(fl+'_feas.txt','r')
        #num=int(temfile.readline().strip())
        for i in temfile:
            #positions.append(temfile.readline().strip())
            features.append(i.strip())
        
        temfile.close()   
    
    allf=open('all%sFeatures.txt'%(lg),'wb')
    for fea in features:
        print(fea,file=allf)
    
    allf.close()
    return features


if __name__ == "__main__":
#    os.chdir('E:\\TunnelHough')
    
    allf=[]
    allf=allf+loadfea("positive")
    allf=allf+loadfea("negative")
    af=open('allFeatures.txt','wb')
    for fea in allf:
        print(fea,file=af)
    
    af.close()    
	
        