#include <iostream>
#include <chrono>
#include "EntityBasicPropertyTypes.h"
#include "EntityDB.h"
#include "Entity.h"

enum {INT_TYPE, FLOAT_TYPE, STRING_TYPE};

size_t UNIQUE_ID_01 = 0x01;
size_t UNIQUE_ID_02 = 0x02;
size_t UNIQUE_ID_03 = 0x03;
size_t UNIQUE_ID_04 = 0x04;
size_t UNIQUE_ID_05 = 0x05;
size_t UNIQUE_ID_06 = 0x06;
size_t UNIQUE_ID_07 = 0x07;

void printData(EntityDataInstance *data) {

	std::vector<size_t> lst = data->getPropertyList();

	std::cout << "Printing Data\n";
	for (auto id : lst) {

		if (data->getPropertyType(id) == EntityPropertyTypes::iINT_TYPE) {
			std::cout << data->getPropertyName(id) << ": " << *(int*)data->getProperty(id) << std::endl;
		}
		else if (data->getPropertyType(id) == EntityPropertyTypes::iSTRING_TYPE) {
			std::cout << data->getPropertyName(id) << ": " << *(std::string*)data->getProperty(id) << std::endl;
		}

	}
}

//class TestObject: public Entity {
//public:
//	static EntityID fnameID;
//	static EntityID lnameID;
//	static EntityID ageID;
//public:
//	TestObject(size_t unique) :Entity(unique) {
//
//	}
//
//	virtual void onDataChanged(EntityDataInstance *instance) {
//		std::cout << "Data Changed\n";
//	};
//
//	static void initialize() {
//		fnameID = EntityDB::createProperty("first_name", new EntityPropertyTypes::StringPropertyType);
//		lnameID = EntityDB::createProperty("last_name", new EntityPropertyTypes::StringPropertyType);
//		ageID = EntityDB::createProperty("age", new EntityPropertyTypes::IntPropertyType);
//	};
//};
//
//EntityID TestObject::fnameID;
//EntityID TestObject::lnameID;
//EntityID TestObject::ageID;
//
//
//class TestObject2 : public TestObject {
//public:
//	static EntityID addr_line_oneID;
//	static EntityID addr_line_twoID;
//	static EntityID provinceID;
//	static EntityID countryID;
//	
//public:
//	TestObject2(size_t unique): TestObject(unique) {
//
//	}
//
//	virtual void onDataChanged(EntityDataInstance *instance) {
//		std::cout << "Data Changed\n";
//	};
//
//	static void initialize() {
//		TestObject::initialize();
//
//		addr_line_oneID = EntityDB::createProperty("address_line_one", new EntityPropertyTypes::StringPropertyType);
//		addr_line_twoID = EntityDB::createProperty("address_line_two", new EntityPropertyTypes::StringPropertyType);
//		provinceID = EntityDB::createProperty("province", new EntityPropertyTypes::StringPropertyType);
//		countryID = EntityDB::createProperty("country", new EntityPropertyTypes::StringPropertyType);
//	};
//};
//
//EntityID TestObject2::addr_line_oneID;
//EntityID TestObject2::addr_line_twoID;
//EntityID TestObject2::provinceID;
//EntityID TestObject2::countryID;
//

class Node : public Entity {
private:
	
public:
	Node(size_t globalid): Entity(globalid) {

	}

	void compute() override {
		//std::cout << "Value: " << index << std::endl;
	}

	void onDataChanged(EntityDataInstance *instance) override {
		
	}

	static void initialize() {}
};

class ScalarNode : public Entity {
public:
	static EntityID valueID;

public:

	ScalarNode(size_t globalid): Entity(globalid){
		this->setPropertyState(valueID, EntityPropertyState::OUTPUT_STATE);
	}

	void compute() override {
		getDataInstance()->makeClean();
	}

	void onDataChanged(EntityDataInstance *instance) override {
		//std::cout << "Data Changed\n";
	};

	static void initialize() {
		
		valueID = EntityDB::createProperty("scalar", new EntityPropertyTypes::IntPropertyType);
	};
};

EntityID ScalarNode::valueID;

class AddNode : public Entity {
public:
	static EntityID inputAID;
	static EntityID inputBID;
	static EntityID outputID;

public:

	AddNode(size_t globalid) :Entity(globalid) {

		setPropertyState(inputAID, EntityPropertyState::INPUT_STATE);
		setPropertyState(inputBID, EntityPropertyState::INPUT_STATE);
		setPropertyState(outputID, EntityPropertyState::OUTPUT_STATE);
	}

	void compute() override {

		int *output = (int*)getProperty(outputID);
		int a = *(int*)getProperty(inputAID);
		int b = *(int*)getProperty(inputBID);
		*output = a + b;

		getDataInstance()->makeClean();
	}

	void onDataChanged(EntityDataInstance *instance) override {
		//std::cout << "Data Changed\n";
	};

	static void initialize() {

		inputAID = EntityDB::createProperty("A", new EntityPropertyTypes::IntPropertyType);
		inputBID = EntityDB::createProperty("B", new EntityPropertyTypes::IntPropertyType);
		outputID = EntityDB::createProperty("C", new EntityPropertyTypes::IntPropertyType);
	};
};

EntityID AddNode::inputAID;
EntityID AddNode::inputBID;
EntityID AddNode::outputID;

class DisplayNode : public Entity {
public:
	static EntityID inputID;

public:

	DisplayNode(size_t globalid) :Entity(globalid) {

		setPropertyState(inputID, EntityPropertyState::INPUT_STATE);
	}

	void compute() override {
		std::cout << "Final: " << *(int*)getProperty(inputID) << std::endl;
		getDataInstance()->makeClean();
	}

	void onDataChanged(EntityDataInstance *instance) override {
		//std::cout << "Data Changed\n";
	};

	static void initialize() {

		inputID = EntityDB::createProperty("scalar", new EntityPropertyTypes::IntPropertyType);
	};
};

EntityID DisplayNode::inputID;

void main() {

	EntityDB::registerEntity(UNIQUE_ID_04, Node::initialize);
	EntityDB::registerEntity(UNIQUE_ID_01, ScalarNode::initialize);
	EntityDB::registerEntity(UNIQUE_ID_02, AddNode::initialize);
	EntityDB::registerEntity(UNIQUE_ID_03, DisplayNode::initialize);

	Entity *root = new Node(UNIQUE_ID_04);
	Entity *a = new ScalarNode(UNIQUE_ID_01);
	Entity *b = new ScalarNode(UNIQUE_ID_01);
	Entity *add = new AddNode(UNIQUE_ID_02);
	Entity *display = new DisplayNode(UNIQUE_ID_03);

	
	*((int*)a->getProperty(ScalarNode::valueID)) = 5;
	*((int*)b->getProperty(ScalarNode::valueID)) = 2;

	Entity::bind(a, ScalarNode::valueID,
			     add, AddNode::inputAID);

	Entity::bind(b, ScalarNode::valueID,
				 add, AddNode::inputBID);

	Entity::bind(add, AddNode::outputID,
				 display, DisplayNode::inputID);

	root->addChild(a);
	root->addChild(b);
	root->bfsQuery(root);

	//std::cout << "Test: " << *((int*)add->getProperty(ScalarNode::valueID)) << std::endl;
	
	/*EntityDataListener lsn01( [](EntityDataEvent evt) {
		if(evt.getType() == EntityDataEventType::FINISHED_EVENT)
			std::cout << " Whoo Hoo!! Finished job!!!\n\n";
		else
			std::cout << " Whoo Hoo!! Starting job!!!\n\n";
	});

	EntityDB::registerEntity(UNIQUE_ID_01, TestObject2::initialize);
	
	TestObject2 obj(UNIQUE_ID_01);
	
	obj.getDataInstance()->addEventListener(lsn01);

	obj.getDataInstance()->beginNotify();

	std::string *fname = (std::string*)obj.getProperty(TestObject2::fnameID);
	*fname = "Clark";

	std::string *lname = (std::string*)obj.getProperty(TestObject2::lnameID);
	*lname = "Kent";

	int *age = (int*)obj.getProperty(TestObject2::ageID);
	*age = 30;

	std::string *ln_one = (std::string*)obj.getProperty(TestObject2::addr_line_oneID);
	*ln_one = "Kwacha Rd 3667";

	std::string *ln_two = (std::string*)obj.getProperty(TestObject2::addr_line_twoID);
	*ln_two = "Olympia Park";

	std::string *prov = (std::string*)obj.getProperty(TestObject2::provinceID);
	*prov = "Lusaka";

	std::string *country = (std::string*)obj.getProperty(TestObject2::countryID);
	*country = "Country";

	printData(obj.getDataInstance());

	obj.getDataInstance()->endNotify();*/

	//obj.getDataInstance()->endNotify();

	//std::this_thread::sleep_for(std::chrono::seconds(5));
}