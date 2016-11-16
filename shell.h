#include <vector>
#include <iostream>
#include <sstream>
#include <sys/wait.h>
#include <stdlib.h>
#include<iomanip>
#include <unistd.h>
#include <stdio.h>
#include<cstring>
#include<cstdlib>
#include<fstream>
#include <fcntl.h>  
#include <unistd.h>
#include<exception>
using namespace std;




class IORedirections{
	public:
		char redirectMode[32];
                char redirectToFile[256];
		static const char inputRedirect[32];
		static const char outputRedirect[32];
		static const char outputAppend[32];
		static const char errorRedirect[32];
		static const char errorAppend[32];
		void setRedirectMode(char *mode);
		bool setFile(char *fileName);
		char *getMode(){
			return redirectMode; 
		}
		
		char *getFile(){
			return redirectToFile;
		}
};

const char IORedirections::inputRedirect[32] = "<";
const char IORedirections::outputRedirect[32] = ">";
const char IORedirections::outputAppend[32] = ">>";
const char IORedirections::errorRedirect[32] = "e>";
const char IORedirections::errorAppend[32] = "e>>";


class BookKeeping{
	public:
		string command;
		size_t jobId;
		string statusValue;
		int status;
		void updateBook();
		void displayContent();
};

vector<BookKeeping *> allJobs;


void BookKeeping::displayContent(){
	cout<<setw(10)<<jobId<<setw(20)<<statusValue<<setw(50)<<command<<endl;

}

void BookKeeping::updateBook(){
	int status_new;

	// WNOHANG is added so that the command should not wait for chage of state
    
	size_t w = waitpid(jobId, &status_new, WNOHANG | WUNTRACED | WCONTINUED);

	if(status==status_new){
		return;
	}

	status = status_new;
	if (WIFEXITED(status_new)) {
		cout<<command<<" EXITED"<<endl;
		statusValue="EXITED";
	
	} else if (WIFSIGNALED(status_new)) {
		cout<<command<<" KILLED"<<endl;
		statusValue="KILLED";
	} else if (WIFSTOPPED(status_new)) {
		statusValue="STOPPED";
		cout<<command<<" STOPPED"<<endl;
	} else if (WIFCONTINUED(status_new)) {
		statusValue="CONTINUED";
		cout<<command<<" CONTINUED"<<endl;
	}
}


class LabFunctions
{
private:
        static const string stdioe_signatures[];
public:
        static BookKeeping * show_pipes_info(string str);
        static string preprocess(string str);
        static vector<string> split(string str, char delimiter);
};

const string LabFunctions::stdioe_signatures[] = {"<","e>>","e>",">>",">"};


