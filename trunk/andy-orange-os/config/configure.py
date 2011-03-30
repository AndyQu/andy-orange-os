#!/usr/bin/python

import os;

class MakefileGenerator:
	def __init__(self):
		a=1;
	def generate(self):
		#Define .c source file folders
		Kernel_Dir="./kernel/";
		Lib_Dir="./lib/";
		Systask_Dir="./sys_task/";
		Application_Dir="./application/";
		sourceDirSet=list();
		sourceDirSet+=self.getAllDirs(Kernel_Dir);
		sourceDirSet+=self.getAllDirs(Lib_Dir);
		sourceDirSet+=self.getAllDirs(Systask_Dir);
		sourceDirSet+=self.getAllDirs(Application_Dir);
		#get parameters, which is the parameters for "gcc -M"
		Include_Dir="./include/";
		parameters=" -I "+Kernel_Dir+Include_Dir;
		include_dirs_set=self.getAllDirs(Include_Dir);
		for dir in include_dirs_set:
			parameters+=" -I "+dir;
		#get the dependency relations of kernel & lib .c and .asm files
		#cFileRuleSet is the list for the c file rules
		#asmFileRuleSet is the list for the assemble file rules
		tmpFile="./mk.tmp"
		cFileRuleSet=list();
		asmFileRuleSet=list();
		objFiles="";
		for sourceDir in sourceDirSet:
			source_file_set=os.listdir(sourceDir);
			for source_file in source_file_set:
				filePath=sourceDir+source_file;
				if os.path.isfile(filePath):
					#deal with c source file
					if filePath.endswith(".c"):
						#get object File name
						objectFile=filePath.replace(".c",".o");
						objFiles+=objectFile+" ";
						#generate c object file rule,via "gcc -M" command option
						#!!!Because the generated object file name doesn't include parent folder name,So it must be added to the rule manually
						cmd="gcc -M "+filePath+parameters+" > "+tmpFile;
						ret=os.system(cmd);
						if ret!=0:
							print "gcc -M error"
						else:
							rule=sourceDir;#the parent folder name is added here to the rule
							f=file(tmpFile,"r");
							tmp=f.readline()
							while tmp!="":
								rule+=tmp;
								tmp=f.readline();
							rule+="\t$(CC) $(CFLAGS) -o $@ $<\n"
							cFileRuleSet.append(rule);
					#deal with assemble source file
					elif filePath.endswith(".asm"):
						#get object File name
						objectFile=filePath.replace(".asm",".o");
						if filePath.endswith("kernel.asm")==False:
							objFiles+=objectFile+" ";
						#generate asm object file rule
						rule=objectFile+": "+filePath;
						if source_file=="kernel.asm":
							rule+=" include/pm.inc";#kernel.asm needs "pm.inc" file
						rule+="\n\t$(ASM) $(ASM_KERNEL_FLAGS) -o $@ $<\n"
						asmFileRuleSet.append(rule);
		#delete temporary file
		os.system("rm -f "+tmpFile);
		#generate "OBJS" variable
		objFiles="OBJS=./kernel/kernel.o "+objFiles+"\n"
		#generate makefile
		#"CFLAG" variable must be modified
		MakeFileName="./makefile"
		basefile_1="./config/m1_base";
		basefile_2="./config/m2_base";
		basefile_3="./config/m3_base";
		os.system("cp -f "+basefile_1+" "+MakeFileName);
		f=file(MakeFileName,"a");
		f.write(objFiles);
		f.write("CFLAGS=-c -fno-builtin -m32 -fno-stack-protector -Wno-deprecated "+parameters);#
		f.close();
		os.system("cat "+basefile_2+" >> "+MakeFileName);
		os.system("cat "+basefile_3+" >> "+MakeFileName);
		f=file(MakeFileName,"a");
		for s in cFileRuleSet:
			f.write(s);
		for s in asmFileRuleSet:
			f.write(s);
		f.close();
		#testdir=self.getAllDirs("/root/Andy/");
		#for a in testdir:
		#	print a;
	#getAllDirs will return a list containing all folders that are under the given folder.
	#the parent folder included
	def getAllDirs(self,basedir):
		retdirs=list();
		retdirs.append(basedir);
		dirs=os.listdir(basedir);
		for dir in dirs:
			dir=basedir+'/'+dir+'/';
			if os.path.isdir(dir):
				retdirs+=(self.getAllDirs(dir));
		return retdirs;
generator=MakefileGenerator();
generator.generate();
