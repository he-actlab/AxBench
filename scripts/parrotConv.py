#!/bin/usr/python

# Amir Yazdanbakhsh
# Jan. 7, 2015

import os
import sys
import shutil
import re

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def printUsage():
    print "python parrotConv.py <bench name>"
    exit(1)

def findFileName(filename):
    matchObj = re.match( r'()', filename)
pass;

def listCppFiles(dir):
    fileList = []
    extList  = []
    for f in os.listdir(dir):
        if(".c" in f):
            extLoc = f.rfind(".")
            fileList.append(f[:-(len(f)-extLoc)])
            extList.append(f[extLoc+1:])
    return (fileList, extList)
pass;

def parseParrotArgs(args):
        args = re.sub(',\s+', ',', args)
        args = re.sub('\s+$', '', args)
        args = args.split(',')

        parrotArgs = []
        for a in args:
            #print '-'*32
            #print a
            m = re.match('^\s*(\[(.+)\])?\s*(<(.+);(.+)>)?\s*(.+)$', a)

            features = [m.group(6), m.group(2), m.group(4), m.group(5)]
            #print features
            for i in range(len(features)):
                if ((features[i] == None) and (i != 1)):
                    features[i] = '0'
                pass
            
                if (features[i] == None):
                    continue
            
                features[i] = re.sub('^\s+', '', features[i])
                features[i] = re.sub('\s+$', '', features[i])
            pass

            parrotArgs.append((features[0], features[1], (features[2], features[3])))
            #print parrotArgs[-1]
        pass
    
        return parrotArgs
pass;

def parseParrotPragma(line, keyword):
    # process pragma
    m = re.match(
                    '#pragma\s+' +
                    'parrot' +
                    '\s*\(\s*' +
                    keyword +
                    '\s*,\s*' +
                    '"(.+)"' +
                    '\s*,\s*' +
                    '(.+)\)',
                    line
                )
                    
    parrotArgs = parseParrotArgs(m.group(2))
    return parrotArgs
pass;

def sobelReplacement(line):
    if("std::sqrt(256 * 256 + 256 * 256)" in line):
        return line.replace("std::sqrt(256 * 256 + 256 * 256)", "256")
    return line

pass;

def main():

    bench_name = sys.argv[1]

    # search src directory for all the c | cpp files
    src_dir = os.getcwd() + "/src"
    obj_dir = os.getcwd() + "/obj"
    nn_dir  = os.getcwd() + "/src.nn"
    (fileList, extList) = listCppFiles(src_dir)
    startPargma = False

    for i in range(len(fileList)):
        parrotoFile = fileList[i] + ".o.parroto." + extList[i] + ".tmp"
        if(".tmp" in parrotoFile):
            currFile = open(obj_dir + "/" + parrotoFile)
            lines = currFile.readlines()
            fileStr = "#include \"fann.h\"\n"

            for line in lines:
                line = line.rstrip()

                # customized options for each benchmarks
                if(bench_name == "sobel"):
                    line = sobelReplacement(line)

                # start of pragma
                if(("#pragma" in line) and not startPargma):

                    (varName, varLen, varRange) = parseParrotPragma(line, 'input')[0]

                    fileStr += "%s %s\n" % ("//", line)

                    # add necessary lines for FANN
                    fileStr += "\tstruct fann *ann;\n"
                    fileStr += "\tfann_type *parrotOut;\n"

                    # copy data from varName to ParrotIn
                    if(type(varLen) != type(None)): # scalar variable
                        fileStr += "\tfann_type parrotIn[%s];\n" % (varLen)
                        for j in range(int(varLen)):
                            fileStr += "\tparrotIn[%d] = %s[%d];\n" % (j, varName, j)
                    else: #single variable 
                            fileStr += "\tfann_type parrotIn[1];\n"
                            fileStr += "\tparrotIn[0] = %s;\n" % (varName)

                    fileStr += "\tann = fann_create_from_file(\"fann.config/%s.nn\");\n" % (bench_name)
                    fileStr += "\tparrotOut = fann_run(ann, (fann_type*)parrotIn);\n";
                    #----------------------------
                    startPargma = True
                    continue
                if(("#pragma" in line) and startPargma): # start of pragma
                    startPargma = False
                    fileStr += "%s %s\n" % ("//", line)
                    (varName, varLen, varRange) = parseParrotPragma(line, 'output')[0]

                    # copy data from varName to ParrotIn
                    if(type(varLen) != type(None)): # scalar variable
                        #print varLen
                        for j in range(int(varLen)):
                            fileStr += "\t%s[%d] = parrotOut[%d];\n" % (varName, j, j)
                    else: #single variable 
                            fileStr += "\t%s = parrotOut[0];\n" % (varName)

                    continue


                # check if we are between pragmas
                if(startPargma):
                    fileStr += "%s %s\n" % ("//", line)
                else:
                    fileStr += line + "\n"

            dstFilename   = fileList[i] + "_nn." + extList[i]
            dstFileHandle = open(nn_dir + "/" + dstFilename, "w")
            dstFileHandle.write(fileStr) 

if __name__ == "__main__":
    main()
