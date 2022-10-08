class Person {
	private:
		int Id;
		char FullName[100];
		int Age;
	public:
		Person() {
			this->Id = 0;
			this->FullName[100];
			this->Age = 0;
		}
		Person(int Id, char FullName[100], int Age) {
			this->Id = Id;
			this->FullName[100] = FullName[100];
			this->Age = Age;
		}
		void setId(int Id) {
			this->Id = Id;
		}
		int getId() {
			return this->Id;
		}
		void setFullName(char FullName[100]) {
			this->FullName[100] = FullName[100];
		}
		char getFullName() {
			return this->FullName[100];
		}
		void setAge(int Age) {
			this->Age = Age;
		}
		int getAge() {
			return this->Age;
		}
		string toString() {
			std::stringstream out;
			out<<"Id: "<<this->Id<<endl
			   <<"Full Name: "<<this->FullName<<endl
			   <<"Age: "<<this->Age<<endl
			   <<this->thisPersonIs();
			return out.str();
		}
		string thisPersonIs() {
			if(this->Age > 0 && this->Age < 4) { return "I'm a baby"; }
			if(this->Age > 3 && this->Age < 11) { return "I'm a child"; }
			if(this->Age > 10 && this->Age < 19) { return "I'm an adolecent"; }
			if(this->Age > 18 && this->Age < 39) { return "I'm an adult"; }
			if(this->Age > 40 && this->Age < 59) { return "I'm an upper adult"; }
			if(this->Age > 60 && this->Age < 100) { return "I'm an ancian"; }
		}
		~Person() {
			// destruye
		}
};
