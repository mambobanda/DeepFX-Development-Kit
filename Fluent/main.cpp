#include <chrono>
#include "FlUtil.h"
#include "FlSpace.h"
#include "FlAnyMap.h"
#include "FlFlow.h"
#include "FlObserver.h"
#include "FlInterpreter.h"
#include "FlDispatchManager.h"
#include "FlDispatcher.h"

void sync(fl::SpaceInterface *space, fl::DataObject* dobj) {

	
}

void compute_scalar(fl::SpaceInterface *space, fl::DataObject* dobj) {

	auto prop_a = space->dataObjectProperty(dobj, "a");
	auto prop_b = space->dataObjectProperty(dobj, "b");

	if (prop_a.size() > 0 && prop_b.size() > 0) {
		prop_b.setData<float>(0, prop_a.data<float>(0));
	}

	space->syncDataObject(dobj);
}

void compute_sum(fl::SpaceInterface *space, fl::DataObject* dobj) {

	auto prop_a = space->dataObjectProperty(dobj, "a");
	auto prop_b = space->dataObjectProperty(dobj, "b");
	auto prop_c = space->dataObjectProperty(dobj, "c");
	
	if (prop_a.size() > 0 && prop_b.size() > 0 && prop_c.size() > 0) {

		float result = prop_a.data<float>(0) + prop_b.data<float>(0);
		prop_c.setData<float>(0, result);

		printf("Sum Result: %f\n", prop_c.data<float>(0));
	}

	space->syncDataObject(dobj);
}

class Observer01 : public fl::Observer {
public: 
	void update(fl::Any* any) override {
		std::cout << "Observer 01 Value Updated:  " << any->data<float>() << std::endl;
	}
};

class Observer02 : public fl::Observer {
public:
	void update(fl::Any* any) override {
		std::cout << "Observer 02 Value Updated:  " << any->data<float>() << std::endl;
	}
};

class Observer03 : public fl::Observer {
public:
	void update(fl::Any* any) override {
		std::cout << "Observer 03 Value Updated:  " << any->data<float>() << std::endl;
	}
};

class Observer04 : public fl::Observer {
public:
	void update(fl::Any* any) override {
		std::cout << "Observer 04 Value Updated:  " << any->data<float>() << std::endl;
	}
};

/// Bool Scalar Node
void boolScalarMethod(fl::SpaceInterface *space, fl::DataObject *dao, fl::InterpreterScope *scope) {
	if (!dao->hasProperty("a")) return;
	if (!dao->hasProperty("b")) return;

	auto _a = scope->fetchInput(dao->property("a")->absolutePath())[0];

	bool a = _a->data<bool>();

	auto result = new fl::Any();
	result->newData<bool>(fl::type::BOOL);
	result->setData<bool>(a);

	scope->addOutput(dao->property("b")->absolutePath(), result);
}

fl::Any *boolScalarNode(fl::SpaceInterface *space) {

	auto mnode = fl::DataObject::Builder()
		.setName("bool_scalar")
		.setLabel("Bool Scalar")
		.setMode(fl::DataObject::NODE)
		.addProperty(space, fl::type::BOOL, fl::role::INPUT, "a", "A", false)
		.addProperty(space, fl::type::BOOL, fl::role::OUTPUT, "b", "B", false)
		.addFunction(fl::DataObject::MAIN, boolScalarMethod)
		.build(space);

	return mnode;
}

/// Scalar Node
void scalarMethod(fl::SpaceInterface *space, fl::DataObject *dao, fl::InterpreterScope *scope) {
	if (!dao->hasProperty("a")) return;
	if (!dao->hasProperty("b")) return;

	auto _a = scope->fetchInput(dao->property("a")->absolutePath())[0];

	float a = _a->data<float>();

	auto result = new fl::Any();
	result->newData<float>(fl::type::FLOAT);
	result->setData<float>(a);

	scope->addOutput(dao->property("b")->absolutePath(), result);
}

fl::Any *scalarNode(fl::SpaceInterface *space) {

	auto mnode = fl::DataObject::Builder()
		.setName("scalar")
		.setLabel("Scalar")
		.setMode(fl::DataObject::NODE)
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "a", "A", 10.f)
		.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "b", "B", 0.f)
		.addFunction(fl::DataObject::MAIN, scalarMethod)
		.build(space);

	return mnode;
}

/// Multiply Node
void multiplyMethod(fl::SpaceInterface *space, fl::DataObject *dao, fl::InterpreterScope *scope) {
	if (!dao->hasProperty("a")) return;
	if (!dao->hasProperty("b")) return;
	if (!dao->hasProperty("c")) return;

	auto _a = scope->fetchInput(dao->property("a")->absolutePath())[0];
	auto _b = scope->fetchInput(dao->property("b")->absolutePath())[0];

	float a = _a->data<float>();
	float b = _b->data<float>();
	
	auto result = new fl::Any();
	result->newData<float>(fl::type::FLOAT);
	result->setData<float>(a * b);

	printf("//////\nMULTIPLY METHOD RESULT: %f\n//////\n\n", a * b);

	scope->addOutput(dao->property("c")->absolutePath(), result);
}

fl::Any *multiplyNode(fl::SpaceInterface *space) {

	auto mnode = fl::DataObject::Builder()
		.setName("multiply")
		.setLabel("Multiply")
		.setMode(fl::DataObject::NODE)
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "a", "A", 5.f)
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "b", "B", 2.f)
		.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT,"c", "Result", 0.f)
		.addFunction(fl::DataObject::MAIN, multiplyMethod)
		.build(space);

	return mnode;
}

/// Square Composite Node
//fl::Any *squareNode(fl::SpaceInterface *space) {
//
//	auto snode = fl::DataObject::Builder()
//		.setName("square")
//		.setLabel("Square")
//		.setMode(fl::DataObject::COMPOUND_NODE)
//		.addProperty(space, fl::type::COMPOUND_PORT, fl::role::COMPOUND_INPUT, "a", "A", 0)
//		.addProperty(space, fl::type::COMPOUND_PORT, fl::role::COMPOUND_OUTPUT, "b", "B", 0)
//		.build(space);
//
//	auto mult_node = multiplyNode(space);
//
//	space->move(mult_node, snode);
//
//	space->addBinding(
//		snode->rawData<fl::DataObject>()->property("a")->absolutePath(),
//		mult_node->rawData<fl::DataObject>()->property("a")->absolutePath());
//
//	space->addBinding(
//		snode->rawData<fl::DataObject>()->property("a")->absolutePath(),
//		mult_node->rawData<fl::DataObject>()->property("b")->absolutePath());
//
//	space->addBinding(
//		mult_node->rawData<fl::DataObject>()->property("c")->absolutePath(),
//		snode->rawData<fl::DataObject>()->property("b")->absolutePath());
//
//	return snode;
//}

class TestDisptacher: public fl::Dispatcher {
public:
	void valueChanged(fl::Any *any) {
		std::cout << "Test Dispatcher value changed: " << any->absolutePath() <<"\n";
	}
};


namespace xtimer {
	class Timer {
	private:
		std::chrono::system_clock::time_point start;
		double finalTime;

	public:

		void startTimer() {
			start = std::chrono::system_clock::now();
		}

		void stopTimer() {
			auto diff = std::chrono::system_clock::now() - start;
			finalTime = std::chrono::duration <double, std::milli>(diff).count();
		}

		double getTime() {
			return finalTime;
		}
	};
}



void main() {

	//fl::Space *space = new fl::Space;

	//auto global = fl::DataObject::Builder()
	//	.setName("GLOBAL")
	//	.setLabel("global")
	//	.build(space);

	//auto objects = fl::DataObject::Builder()
	//	.setName("thing")
	//	.setLabel("Objects")
	//	.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "name", "Name", 0.f)
	//	.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "age", "Age", 0.f)
	//	.build(space);

	//space->move(objects, global);

	//xtimer::Timer t00;
	//t00.startTimer();
	//
	//fl::util::StringSplitter splitter("~/Global/Acme/Test");
	//
	//for (size_t inx = 0; inx < 500; inx++) {
	//	auto any = space->findAbsolute("~/GLOBAL/thing");
	//	/*std::string first = splitter.first();
	//	std::string next = splitter.next();

	//	bool splittable = splitter.isSplittable('/');
	//	auto count = splitter.count();*/
	//}

	//t00.stopTimer();

	//std::cout << "Time: " << t00.getTime() << std::endl;

	//if (any != nullptr) std::cout << "Found\n";
	/*fl::Space *space = new fl::Space;

	auto any00 = space->any("any00", "Any A");
	auto any01 = space->any("any01", "Any B");
	auto any02 = space->any("any02", "Any C");
	auto any03 = space->any("any03", "Any D");

	fl::Dispatcher *dispatcher = new TestDisptacher;
	fl::DispatchManager::getInstance()->addDispatcher(dispatcher);

	dispatcher->attachAny(any00);
	dispatcher->attachAny(any01);
	dispatcher->attachAny(any02);
	dispatcher->attachAny(any03);

	any00->newData<int>(1);
	any01->newData<int>(1);
	any02->newData<int>(1);
	any03->newData<int>(1);*/

	/*fl::Space *space = new fl::Space;

	auto objects = fl::DataObject::Builder()
		.setName("OBJS")
		.setLabel("Objects")
		.build(space);

	auto bigObjects = fl::DataObject::Builder()
		.setName("BIG_OBJS")
		.setLabel("Big Objects")
		.build(space);

	auto smallObjects = fl::DataObject::Builder()
		.setName("SMALL_OBJS")
		.setLabel("Small Objects")
		.build(space);

	auto bigAny00 = space->any("object", "Object A");
	space->move(bigAny00, bigObjects);
	auto bigAny01 = space->any("object", "Object B");
	space->move(bigAny01, bigObjects);
	auto bigAny02 = space->any("object", "Object C");
	space->move(bigAny02, bigObjects);

	auto smallAny00 = space->any("object", "Object B");
	space->move(smallAny00, smallObjects);
	auto smallAny01 = space->any("object", "Object A");
	space->move(smallAny01, smallObjects);

	space->move(bigObjects, objects);
	space->move(smallObjects, objects);

	std::cout << "<< Folders >>" << std::endl;

	std::cout << objects->absolutePath() << std::endl;

	std::cout << "\t--" << bigObjects->absolutePath() << std::endl;
	std::cout << "\t\t--"<< bigAny00->absolutePath() << std::endl;
	std::cout << "\t\t--" << bigAny01->absolutePath() << std::endl;
	std::cout << "\t\t--" << bigAny02->absolutePath() << std::endl;

	std::cout << "\t--" << smallObjects->absolutePath() << std::endl;
	std::cout << "\t\t--" << smallAny00->absolutePath() << std::endl;
	std::cout << "\t\t--" << smallAny01->absolutePath() << std::endl;

	std::cout << "\n\n";

	std::cout << "Has Path '~/CAMS/camera01': ";
	if (space->findAbsolute("~/CAMS/camera01") != nullptr) std::cout << "YES...\n\n";
	else std::cout << "NO...\n\n";

	std::cout << "Has Path '~/OBJS/BIG_OBJS/object': ";
	if (space->findAbsolute("~/OBJS/BIG_OBJS/object") != nullptr) std::cout << "YES...\n\n";
	else std::cout << "NO...\n\n";

	std::cout << "Has Path '~/OBJS/BIG_OBJS/object22': ";
	if (space->findAbsolute("~/OBJS/BIG_OBJS/object22") != nullptr) std::cout << "YES...\n\n";
	else std::cout << "NO...\n\n";

	std::cout << "Has Path '~/OBJS/BIG_OBJS/{Object A}': ";
	if (space->findAbsolute("~/OBJS/BIG_OBJS/{Object A}") != nullptr) std::cout << "YES...\n\n";
	else std::cout << "NO...\n\n";

	std::cout << "Has Path '~/OBJS/BIG_OBJS/{Object F}': ";
	if (space->findAbsolute("~/OBJS/BIG_OBJS/{Object F}") != nullptr) std::cout << "YES...\n\n";
	else std::cout << "NO...\n\n";

	std::cout << "Has Path '~/OBJS/{Small Objects}/{Object A}': ";
	if (space->findAbsolute("~/OBJS/{Small Objects}/{Object A}") != nullptr) std::cout << "YES...\n\n";
	else std::cout << "NO...\n\n";*/
	
	/*fl::Space *space = new fl::Space;
	fl::Flow *flow = new fl::Flow(space);

	auto compute = fl::DataObject::Builder()
		.setName("compute_graph")
		.setLabel("Computation")
		.build(space);

	auto compute_condition = fl::DataObject::Builder()
		.setName("compute_graph_condition")
		.setLabel("Computation")
		.build(space);

	auto mult_node00 = multiplyNode(space);
	auto mult_node01 = multiplyNode(space);
	auto mult_node02 = multiplyNode(space);
	auto mult_node03 = multiplyNode(space);
	auto mult_node04 = multiplyNode(space);

	auto scala_node00 = scalarNode(space);
	auto scala_node01 = scalarNode(space);
	auto scala_node02 = scalarNode(space);
	auto scala_node03 = scalarNode(space);
	auto scala_node04 = scalarNode(space);

	auto square_node = squareNode(space);

	auto bool_scalar_node = boolScalarNode(space);

	mult_node00->setName("multiply_00");
	mult_node01->setName("multiply_01");
	mult_node02->setName("multiply_02");
	mult_node03->setName("multiply_03");
	mult_node04->setName("multiply_04");

	scala_node00->setName("scalar_00");
	scala_node01->setName("scalar_01");
	scala_node02->setName("scalar_02");
	scala_node03->setName("scalar_03");
	scala_node04->setName("scalar_04");

	space->move(mult_node00, compute);
	space->move(mult_node01, compute);
	space->move(mult_node02, compute);
	space->move(mult_node03, compute);
	space->move(mult_node04, compute);

	space->move(scala_node00, compute);
	space->move(scala_node01, compute);
	space->move(scala_node02, compute);
	space->move(scala_node03, compute);
	space->move(scala_node04, compute);

	space->move(square_node, compute);
	space->move(bool_scalar_node, compute_condition);

	space->addBinding(
		scala_node00->rawData<fl::DataObject>()->property("b")->absolutePath(),
		mult_node00->rawData<fl::DataObject>()->property("a")->absolutePath());

	space->addBinding(
		scala_node01->rawData<fl::DataObject>()->property("b")->absolutePath(),
		mult_node00->rawData<fl::DataObject>()->property("b")->absolutePath());

	space->addBinding(
		scala_node02->rawData<fl::DataObject>()->property("b")->absolutePath(),
		mult_node01->rawData<fl::DataObject>()->property("a")->absolutePath());

	space->addBinding(
		mult_node00->rawData<fl::DataObject>()->property("c")->absolutePath(),
		mult_node01->rawData<fl::DataObject>()->property("b")->absolutePath());

	space->addBinding(
		mult_node00->rawData<fl::DataObject>()->property("c")->absolutePath(),
		mult_node02->rawData<fl::DataObject>()->property("a")->absolutePath());

	space->addBinding(
		mult_node00->rawData<fl::DataObject>()->property("c")->absolutePath(),
		mult_node03->rawData<fl::DataObject>()->property("a")->absolutePath());

	space->addBinding(
		scala_node03->rawData<fl::DataObject>()->property("b")->absolutePath(),
		mult_node03->rawData<fl::DataObject>()->property("b")->absolutePath());

	space->addBinding(
		mult_node02->rawData<fl::DataObject>()->property("c")->absolutePath(),
		mult_node04->rawData<fl::DataObject>()->property("a")->absolutePath());

	space->addBinding(
		mult_node03->rawData<fl::DataObject>()->property("c")->absolutePath(),
		mult_node04->rawData<fl::DataObject>()->property("b")->absolutePath());

	space->addBinding(
		mult_node04->rawData<fl::DataObject>()->property("c")->absolutePath(),
		scala_node04->rawData<fl::DataObject>()->property("a")->absolutePath());

	space->addBinding(
		scala_node04->rawData<fl::DataObject>()->property("b")->absolutePath(),
		square_node->rawData<fl::DataObject>()->property("a")->absolutePath());

	auto p1 = flow->addProcess();

	flow->addProcessAction(p1, compute->absolutePath());
	flow->addProcessAction(p1, "~/GLOBAL/graphs/b");
	flow->addProcessAction(p1, "~/GLOBAL/graphs/c");

	auto d1 = flow->addDecision();

	auto decision_out = space->findAbsolute(flow->node(d1)->conditionOuputNodePath());

	flow->setDecisionConditionPath(d1, compute_condition->absolutePath());
	space->addBinding(
		bool_scalar_node->rawData<fl::DataObject>()->property("b")->absolutePath(),
		decision_out->rawData<fl::DataObject>()->property("condition")->absolutePath());

	auto p2 = flow->addProcess();
	flow->addProcessAction(p2, "~/GLOBAL/graphs/d");

	auto b1 = flow->addBreak();

	flow->start()->setNext(p1);
	flow->node(p1)->setNext(d1);
	flow->node(p2)->setNext(b1);
	flow->node(d1)->setYes(p2);
	flow->node(d1)->setNo(b1);

	fl::Interpreter interpreter(space, flow);
	interpreter.setPrintDebugInfo(true);
	interpreter.run();

	delete flow;*/
	/*for (auto n : flow->nodes()) {
		printf("ID: %i\n", n.id());

		if (n.type() == fl::Flow::START) {
			printf("\tType: Start\n");
			printf("\tNext: %i\n\n", n.next());
		}
		else if (n.type() == fl::Flow::PROCESS) {
			printf("\tType: Process\n");
			printf("\tNext: %i\n", n.next());
			for (auto p : n.paths()) printf("\tPath: %s\n", p.c_str());
			printf("\n");
		}
		else if (n.type() == fl::Flow::DECISION) {
			printf("\tType: Decision\n");
			printf("\tNext: %i\n", n.next());
			printf("\tCondition Path: %s\n", n.conditionPath().c_str());
			printf("\tYes: %i\n", n.yes());
			printf("\tNo: %i\n\n", n.no());
		}
		else if (n.type() == fl::Flow::BREAK) printf("\tType: Break\n");
	}*/
	/*auto val = space->any("float", "Float");
	val->newData<float>(fl::type::FLOAT);

	{
		Observer01 obs01;
		Observer02 obs02;
		Observer03 obs03;
		Observer04 obs04;

		val->addObserver(&obs01);
		val->addObserver(&obs02);
		val->addObserver(&obs03);
		val->addObserver(&obs04);

		std::cout << "Observer Count: " << val->observers().size() << "\n\n";

		val->setData<float>(100.f);
		val->notifyObservers();

		std::cout << "\n";

		val->setData<float>(143.f);
		val->notifyObservers(&obs02);

		std::cout << "\n";
	}

	std::cout << "Observer Count: " << val->observers().size() << "\n\n";*/
	/*fl::AnyMap<float, float>  anymap("~/", 5.f);
	anymap.interpolate(5.f, fl::InterpolationType::CONSTANT);*/
	/*fl::Space *space = new fl::Space;

	auto scalar00 = fl::DataObject::Builder()
		.setName("scalar")
		.setLabel("Scalar")
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "a", "input", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "b", "output", 0.f)
		.addFunction("compute", compute_scalar)
		.build(space);

	auto scalar01 = fl::DataObject::Builder()
		.setName("scalar")
		.setLabel("Scalar")
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "a", "input", 2.f)
		.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "b", "output", 0.f)
		.addFunction("compute", compute_scalar)
		.build(space);

	auto scalar02 = fl::DataObject::Builder()
		.setName("scalar")
		.setLabel("Scalar")
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "a", "input", 3.f)
		.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "b", "output", 0.f)
		.addFunction("compute", compute_scalar)
		.build(space);

	auto math_add = fl::DataObject::Builder()
		.setName("add")
		.setLabel("Add")
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT,  "a", "a_in", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT,  "b", "b_in", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "c", "c_out", 0.f)
		.addFunction("compute", compute_sum)
		.build(space);

	auto math_add00 = fl::DataObject::Builder()
		.setName("add")
		.setLabel("Add")
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "a", "a_in", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "b", "b_in", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "c", "c_out", 0.f)
		.addFunction("compute", compute_sum)
		.build(space);

	auto rawView = math_add00->rawData<fl::DataObject>();

	auto sc00_a = fl::DataObject::fromAny(scalar00).property("b");
	auto sc01_a = fl::DataObject::fromAny(scalar01).property("b");
	auto sc02_a = fl::DataObject::fromAny(scalar02).property("b");
	auto math_a = fl::DataObject::fromAny(math_add).property("a");
	auto math_b = fl::DataObject::fromAny(math_add).property("b");
	auto math_c = fl::DataObject::fromAny(math_add).property("c");
	auto math_a00 = fl::DataObject::fromAny(math_add00).property("a");
	auto math_b00 = fl::DataObject::fromAny(math_add00).property("b");

	auto rc = space->findAbsolute(space->absolutePath(math_c));

	std::cout << "sc00_a path: " << space->absolutePath(sc00_a) << std::endl;
	std::cout << "sc01_a path: " << space->absolutePath(sc01_a) << std::endl;
	std::cout << "sc02_a path: " << space->absolutePath(sc02_a) << std::endl;
	std::cout << "math_a path: " << space->absolutePath(math_a) << std::endl;
	std::cout << "math_b path: " << space->absolutePath(math_b) << std::endl;
	std::cout << "math_c path: " << space->absolutePath(math_c) << std::endl;
	std::cout << "math00_a path: " << space->absolutePath(math_a00) << std::endl;
	std::cout << "math00_b path: " << space->absolutePath(math_b00) << std::endl << std::endl;

	space->addBinding(space->absolutePath(sc00_a), space->absolutePath(math_a));
	space->addBinding(space->absolutePath(sc01_a), space->absolutePath(math_b));
	space->addBinding(space->absolutePath(math_c), space->absolutePath(math_a00));
	space->addBinding(space->absolutePath(sc02_a), space->absolutePath(math_b00));
	
	std::vector<std::string> methods{"compute"};
	space->explore(space->root(), methods);*/

	//auto any00 = space.any("A", "Entity");
	//auto any01 = space.any("B", "Entity");
	//auto any02 = space.any("C", "Entity");
	//auto any03 = space.any("D", "Entity");
	//auto any04 = space.any("X", "Entity");

	//any00->newData<float>(fl::type::FLOAT);
	//any00->data<float>() = 5.666f;

	//any04->newData<std::string>(fl::type::STD_STRING);
	//any04->data<std::string>() = "Hello World";

	//any01->newData<fl::DataObject>(fl::type::DATA_OBJECT);
	//any02->newData<fl::DataObject>(fl::type::DATA_OBJECT);
	//any03->newData<fl::DataObject>(fl::type::DATA_OBJECT);

	//space.move(any02, any01);
	//space.move(any03, any02);
	//space.move(any00, any03);
	//space.move(any04, any03);

	//auto s_data = space.root()->rawData<fl::DataObject>();
	//auto a00_data = any01->rawData<fl::DataObject>();

	//std::cout << "Path: " << space.absolutePath(any04) << std::endl;
	//
	//auto ptr = space.findAbsolute(space.absolutePath(any04));
	////auto ptr = space.findRelative(any00, "../../../B");

	//if (ptr != nullptr) {
	//	std::cout << "Success...\n\n";
	//	//std::cout << "Value: " << ptr->data<fl::DataObject>().uid() << std::endl;
	//	std::cout << "Value: " << ptr->data<std::string>().data() << std::endl;
	//}

	/*fl::util::StringSplitter splitter("one/two/three/four/five");

	std::cout << "one/two/three/four/five" << std::endl << std::endl;

	std::cout << "count: " << splitter.count('/') << std::endl << std::endl;
	auto out = splitter.next('/');
	std::cout << out << std::endl;

	fl::util::StringSplitter splitter00(out);
	std::cout << "count: " << splitter00.count('/') << std::endl << std::endl;
	out = splitter00.next('/');
	std::cout << out << std::endl;
	
	fl::util::StringSplitter splitter01(out);
	std::cout << "count: " << splitter01.count('/') << std::endl << std::endl;
	out = splitter01.next('/');
	std::cout << out << std::endl;

	fl::util::StringSplitter splitter02(out);
	std::cout << "count: " << splitter02.count('/') << std::endl << std::endl;
	out = splitter02.next('/');
	std::cout << out << std::endl;

	fl::util::StringSplitter splitter03(out);
	std::cout << "count: " << splitter03.count('/') << std::endl << std::endl;
	out = splitter03.next('/');
	std::cout << out << std::endl;	

	fl::util::StringSplitter splitter04(out);
	std::cout << "count: " << splitter04.count('/') << std::endl << std::endl;
	out = splitter04.next('/');
	std::cout << out << std::endl;*/

	/*fl::Space space;

	auto any00 = space.any("entity", "Entity");
	auto any01 = space.any("entity", "Entity");
	auto any02 = space.any("entity", "Entity");
	auto any03 = space.any("entity", "Entity");

	std::cout << "Any 00: " << any00->name() << std::endl;
	std::cout << "Any 01: " << any01->name() << std::endl;
	std::cout << "Any 02: " << any02->name() << std::endl;
	std::cout << "Any 03: " << any03->name() << std::endl;*/
}