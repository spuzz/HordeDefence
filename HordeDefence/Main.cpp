//#include "Control.h"

#include <iostream>
#include <memory>
#include "Control.h"

using std::cout;
using std::endl;
using std::shared_ptr;
using std::make_shared;

int main(int argc, char* argv[])
{
	shared_ptr<View> mainView(new View());
	shared_ptr<Model> model(new Model());
	
	shared_ptr<Control> control(new Control(model,mainView));
	
	
	cout << "TEST" << endl;
	return 0;
}