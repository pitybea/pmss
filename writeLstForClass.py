import sys
import os

ins=open('folder.config','r')
mydir=ins.readline().strip()
ins.close()  

traindir='\\train\\VOCdevkit\\VOC2007'
testdir='\\test\\VOCdevkit\\VOC2007'

if __name__ == "__main__":   
    print mydir+traindir
