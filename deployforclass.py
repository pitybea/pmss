import os
import sys
import glob

def printAndEx(s):
    print s
    os.system(s)
    
def deploysimple(s,s1,s2):
    cmd='del %s\\%s\\*  /s /f  /q'%(s2,s)
    printAndEx(cmd)
    
    cmd='copy %s\\VOCdevkit\\VOC2007\\JPEGImages\\%s.lst %s\\%s\\'%(s1,s,s2,s)
    printAndEx(cmd)
    
    ins = open('%s\\VOCdevkit\\VOC2007\\JPEGImages\\%s.lst'%(s1,s), "r" )
    array = []
    for line in ins:
        array.append( line.strip() )
    
    fls=array[1:int(array[0])+1]
    for fl in fls:
        cmd='copy %s\\VOCdevkit\\VOC2007\\JPEGImages\\%s.pb %s\\%s\\'%(s1,fl,s2,s)
        printAndEx(cmd)
        cmd='copy %s\\VOCdevkit\\VOC2007\\JPEGImages\\%s.jpg %s\\%s\\'%(s1,fl,s2,s)
        printAndEx(cmd)
        cmd='copy %s\\VOCdevkit\\VOC2007\\Annotations\\%s.xml %s\\%s\\'%(s1,fl,s2,s)
        printAndEx(cmd)        

    
    
if __name__ == '__main__':
    os.chdir('E:\\CarData\\voc2007')
    icd=sys.argv[1]
    deploysimple(icd,'test','testing')
    deploysimple(icd,'train','training')