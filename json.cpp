#include "json.hpp"

struct list{
	json dato;
	list* next;
};

struct dictionary{
	std::pair<std::string,json> elemento;
	dictionary* next;
};

struct json::impl{
	impl(){
		isa=0;
		list_front=list_back=nullptr;
		dictionary_front=dictionary_back=nullptr;
		stringa="";
		numero=0;
		booleano=false;
	}
	list* list_front;
	list* list_back;
	dictionary* dictionary_front;
	dictionary* dictionary_back;
	double numero;
	bool booleano;
	std::string stringa;
	short isa; //0=null, 1=numero, 3=bool, 4=stringa, 5=lista, 6=dizionario
	void delete_all(); //dealloca tutta la memoria e reimposta i dati
	void delete_list(); //dealloca la lista
	void delete_dict(); //dealloca il dizionario
};
void json::impl::delete_list(){
	while(list_front){
		list* temp=list_front;
		list_front=list_front->next;
		delete temp;
	}
	list_front=list_back=nullptr;
}
void json::impl::delete_dict(){
	while(dictionary_front){
		dictionary* temp=dictionary_front;
		dictionary_front=dictionary_front->next;
		delete temp;
	}
	dictionary_front=dictionary_back=nullptr;
}
void json::impl::delete_all(){
	delete_list();
	delete_dict();
	isa=0;
	numero=0;
	booleano=false;
	stringa="";
}


//ITERATORI----------------------------------------------------------------------------------
struct json::list_iterator{
	using iterator_category = std::forward_iterator_tag;
	using value_type = json;
	using pointer = json*;
	using reference = json&;

	list_iterator(list* p) : m_ptr(p) {} //costruttore per begin e end

	reference operator*() const{ return m_ptr->dato; } //ritorno una reference list&
	pointer operator->() const{ return &(m_ptr->dato); } //ritorno un pointer list*
	list_iterator& operator++(){
		m_ptr=m_ptr->next;
		return *this;
	}
	list_iterator operator++(int){
		list_iterator it {m_ptr};
		++(*this);
		return it;
	}
	bool operator==(list_iterator const& rhs) const{ return m_ptr == rhs.m_ptr; } //it1 == it2
	bool operator!=(list_iterator const& rhs) const{ return m_ptr != rhs.m_ptr; }//it1 !=nit2
	//operator bool() const{ return m_ptr != nullptr; } //senza dovremmo scrivere if(it != x.end())
	private:
	list* m_ptr;
};
struct json::const_list_iterator{
	using iterator_category = std::forward_iterator_tag;
	using value_type = json const;
	using pointer = const json*;
	using reference = json const&;

	const_list_iterator(list* p) : m_ptr(p) {} //costruttore per begin e end

	reference operator*() const{ return m_ptr->dato; } //ritorno una reference list const&
	pointer operator->() const{ return &(m_ptr->dato); } //ritorno un pointer list const*
	const_list_iterator& operator++(){
		m_ptr=m_ptr->next;
		return *this;
	}
	const_list_iterator operator++(int){
		const_list_iterator it {m_ptr};
		++(*this);
		return it;
	}
	bool operator==(const_list_iterator const& rhs) const{ return m_ptr == rhs.m_ptr; } //it1 == it2
	bool operator!=(const_list_iterator const& rhs) const{ return m_ptr != rhs.m_ptr; }//it1 !=nit2
	//operator bool() const{ return m_ptr!=nullptr; } //senza dovremmo scrivere if(it != x.end())
	private:
	list* m_ptr;
};

struct json::dictionary_iterator{
	using iterator_category = std::forward_iterator_tag;
	using value_type = std::pair<std::string,json>;
	using pointer = std::pair<std::string,json>*;
	using reference = std::pair<std::string,json>&;

	dictionary_iterator(dictionary* p) : m_ptr(p) {} //costruttore per begin e end

	reference operator*() const{ return m_ptr->elemento; } //ritorno una reference dictionary&
	pointer operator->() const{ return &(m_ptr->elemento); } //ritorno un pointer dictionary*
	dictionary_iterator& operator++(){
		m_ptr=m_ptr->next;
		return *this;
	}
	dictionary_iterator operator++(int){
		dictionary_iterator it {m_ptr};
		++(*this);
		return it;
	}
	bool operator==(dictionary_iterator const& rhs) const{ return m_ptr == rhs.m_ptr; } //it1 == it2
	bool operator!=(dictionary_iterator const& rhs) const{ return m_ptr != rhs.m_ptr; } //it1 !=nit2
	//operator bool() const{ return m_ptr!=nullptr; } //senza dovremmo scrivere if(it != x.end())
	private:
	dictionary* m_ptr;
};
struct json::const_dictionary_iterator{
	using iterator_category = std::forward_iterator_tag;
	using value_type = std::pair<std::string,json> const;
	using pointer = const std::pair<std::string,json>*;
	using reference = std::pair<std::string,json> const&;

	const_dictionary_iterator(dictionary const* p) : m_ptr(p) {} //costruttore per begin e end

	reference operator*() const{ return m_ptr->elemento; } //ritorno una reference dictionary const&
	pointer operator->() const{ return &m_ptr->elemento; } //ritorno un pointer dictionary const*
	const_dictionary_iterator& operator++(){
		m_ptr=m_ptr->next;
		return *this;
	}
	const_dictionary_iterator operator++(int){
		const_dictionary_iterator it {m_ptr};
		++(*this);
		return it;
	}
	bool operator==(const_dictionary_iterator const& rhs) const{ return m_ptr == rhs.m_ptr; } //it1 == it2
	bool operator!=(const_dictionary_iterator const& rhs) const{ return m_ptr != rhs.m_ptr; }//it1 !=nit2
	//operator bool() const{ return m_ptr!=nullptr; } //senza dovremmo scrivere if(it != x.end())
	private:
	dictionary const* m_ptr;
};

json::list_iterator json::begin_list(){
	if(is_list()){
		return {pimpl->list_front};
	}else{
		throw json_exception{"Errore: non è una lista in begin_list"};
	}
}
json::list_iterator json::end_list(){
	if(is_list()){
		return {nullptr};
	}else{
		throw json_exception{"Errore: non è una lista in end_list"};
	}
}
json::const_list_iterator json::begin_list() const{
	if(is_list()){
		return {pimpl->list_front};
	}else{
		throw json_exception{"Errore: non è una lista in begin_list"};
	}
}
json::const_list_iterator json::end_list() const{
	if(is_list()){
		return {nullptr};
	}else{
		throw json_exception{"Errore: non è una lista in end_list"};
	}
}

json::dictionary_iterator json::begin_dictionary(){
	if(is_dictionary()){
		return {pimpl->dictionary_front};
	}else{
		throw json_exception{"Errore: non è un dizionario in begin_dictionary"};
	}	
}
json::dictionary_iterator json::end_dictionary(){
	if(is_dictionary()){
		return {nullptr};
	}else{
		throw json_exception{"Errore: non è un dizionario in end_dictionary"};
	}
}
json::const_dictionary_iterator json::begin_dictionary() const{
	if(is_dictionary()){
		return {pimpl->dictionary_front};
	}else{
		throw json_exception{"Errore: non è un dizionario in begin_dictionary"};
	}	
}
json::const_dictionary_iterator json::end_dictionary() const{
	if(is_dictionary()){
		return {nullptr};
	}else{
		throw json_exception{"Errore: non è un dizionario in end_dictionary"};
	}
}

//-----------------------------------------------------------------------------------------

//default constructor
json::json(){
	pimpl = new impl;
}
//copy constructor
json::json(json const& e) : json(){
	*this=e;
}
//move constructor
json::json(json&& e) : json(){
	*this=std::move(e); //chiamo il move assignment
}
//destructor
json::~json(){
	if(pimpl!=nullptr){
		pimpl->delete_all();
		delete pimpl;
	}
}

//OPERATORI ----------------------------------------------------------------------------------
//0=null, 1=numero, 2=bool, 3=stringa, 4=list, 5=dizionario
json& json::operator=(json const& e){
	if(this!=&e){
		pimpl->delete_all();
		if(e.is_list()){
			set_list();
			list* ptr=e.pimpl->list_front;
			while(ptr){
				push_back(ptr->dato);
				ptr=ptr->next;
			}
		}else if(e.is_dictionary()){
			set_dictionary();
			dictionary* ptr=e.pimpl->dictionary_front;
			while(ptr){
				insert(ptr->elemento);
				ptr=ptr->next;
			}
		}else if(e.is_string()){
			set_string(e.pimpl->stringa);
		}else if(e.is_bool()){
			set_bool(e.pimpl->booleano);
		}else if(e.is_number()){
			set_number(e.pimpl->numero);
		}
	}
	return *this;
}
json& json::operator=(json&& e){
	if(this!=&e){
		if(pimpl){
			pimpl->delete_all();
			delete pimpl;
			pimpl=nullptr;
		}
		pimpl=e.pimpl;
		e.pimpl=nullptr;
	}
	return *this;
}

json const& json::operator[](std::string const& key) const{
	if(is_dictionary()){
		dictionary* diz=pimpl->dictionary_front;
		while(diz){
			if(diz->elemento.first==key){
				return diz->elemento.second;
			}
			diz=diz->next;
		}
		throw json_exception{"Errore: tenta di inserire [] const"};
	}else{
		throw json_exception{"Errore: non è un dizionario in []"};
	}
}
json& json::operator[](std::string const& key){
	if(is_dictionary()){
		dictionary* diz=pimpl->dictionary_front;
		while(diz){
			if(diz->elemento.first==key){
				return diz->elemento.second;
			}
			diz=diz->next;
		}
		json nuovo;
		//nuovo.set_null();
		std::pair<std::string,json> mypair(key,nuovo);
		this->insert(mypair);
		return pimpl->dictionary_back->elemento.second;
	}else{
		throw json_exception{"Errore: non è un dizionario in []"};
	}
}

//FUNZIONI BOOL is_ -------------------------------------------------------------------------------
//0=null, 1=numero, 2=bool, 3=stringa, 4=lista, 5=dizionario
bool json::is_list() const{
	return pimpl->isa==4;
}
bool json::is_dictionary() const{
	return pimpl->isa==5;
}
bool json::is_string() const{
	return pimpl->isa==3;
}
bool json::is_number() const{
	return pimpl->isa==1;
}
bool json::is_bool() const{
	return pimpl->isa==2;
}
bool json::is_null() const{
	return pimpl->isa==0;
}

//FUNZIONI get_ --------------------------------------------------------------------------------
double& json::get_number(){
	if(is_number()){
		return pimpl->numero;
	}else{
		throw json_exception{"Errore: non è un number in get_number()"};
	}
}
double const& json::get_number() const{
	if(is_number()){
		return pimpl->numero;
	}else{
		throw json_exception{"Errore: non è un number in get_number()"};
	}
}
bool& json::get_bool(){
	if(is_bool()){
		return pimpl->booleano;
	}else{
		throw json_exception{"Errore: non è un bool in get_bool()"};
	}
}
bool const& json::get_bool() const{
	if(is_bool()){
		return pimpl->booleano;
	}else{
		throw json_exception{"Errore: non è un bool in get_bool()"};
	}
}
std::string& json::get_string(){
	if(is_string()){
		return pimpl->stringa;
	}else{
		throw json_exception{"Errore: non è una stringa in get_string()"};
	}
}
std::string const& json::get_string() const{
	if(is_string()){
		return pimpl->stringa;
	}else{
		throw json_exception{"Errore: non è una stringa in get_string()"};
	}
}

//FUNZIONI set_ ---------------------------------------------------------------------------
//0=null, 1=numero, 2=bool, 3=stringa, 4=lista, 5=dizionario
void json::set_string(std::string const& x){
	pimpl->delete_all();
	pimpl->isa=3;
	pimpl->stringa=x;
}
void json::set_bool(bool x){
	pimpl->delete_all();
	pimpl->isa=2;
	pimpl->booleano=x;
}
void json::set_number(double x){
	pimpl->delete_all();
	pimpl->isa=1;
	pimpl->numero=x;
}
void json::set_null(){
	pimpl->delete_all();
	pimpl->isa=0;
}
void json::set_list(){
	pimpl->delete_all();
	pimpl->isa=4;
}
void json::set_dictionary(){
	pimpl->delete_all();
	pimpl->isa=5;
}

//PUSH_FRONT PUSH_BACK INSERT------------------------------------------------------------------
void json::push_front(json const& x){
	if(is_list()){
		list* nuovo=new list;
		nuovo->dato=x;
		nuovo->next=pimpl->list_front;
		pimpl->list_front=nuovo;
		if(pimpl->list_back==nullptr) pimpl->list_back=nuovo;
	}else{
		throw json_exception{"Errore: non è una list in push_front()"};
	}
}
void json::push_back(json const& x){
	if(is_list()){
		list* nuovo=new list;
		nuovo->dato=x;
		nuovo->next=nullptr;
		if(pimpl->list_front==nullptr){
			pimpl->list_front=nuovo;
		}
		if(pimpl->list_back!=nullptr){
			pimpl->list_back->next=nuovo;
		}
		pimpl->list_back=nuovo;
	}else{
		throw json_exception{"Errore: non è una list in push_back()"};
	}
}
void json::insert(std::pair<std::string, json> const& x){
	if(is_dictionary()){
		dictionary* nuovo=new dictionary;
		nuovo->elemento=x;
		nuovo->next=nullptr;
		if(pimpl->dictionary_front==nullptr){
			pimpl->dictionary_front=nuovo;
		}
		if(pimpl->dictionary_back!=nullptr){
			pimpl->dictionary_back->next=nuovo;
		}
		pimpl->dictionary_back=nuovo;
	}else{
		throw json_exception{"Errore: non è un dizionario in insert()"};
	}
}

//FUNZIONI ESTERNE A JSON-----------------------------------------------------------------
//STAMPA----------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& lhs, json const& rhs){
	if(rhs.is_null()){
		lhs<<"null";
	} 
	if(rhs.is_bool()){
		if(rhs.get_bool()==true) lhs<<"true";
		if(rhs.get_bool()==false) lhs<<"false";
	}
	if(rhs.is_number()){
		lhs<<rhs.get_number();
	}
	if(rhs.is_string()){
		lhs<<'"'<<rhs.get_string()<<'"';
	}
	if(rhs.is_list()){
		lhs<<'[';
		for(auto ptr=rhs.begin_list(); ptr!=rhs.end_list(); ){
			lhs<<(*ptr);
			++ptr;
			if(ptr!=rhs.end_list()) lhs<<",";
		}
		lhs<<']';
	}
	if(rhs.is_dictionary()){
		lhs<<'{';
		for(auto ptr=rhs.begin_dictionary(); ptr!=rhs.end_dictionary(); ){
			lhs<<'"'<<(*ptr).first<<'"'<<" : "<<(*ptr).second;
			++ptr;
			if(ptr!=rhs.end_dictionary()) lhs<<",";
		}
		lhs<<'}';
	}
	return lhs;
}

//INSERIMENTO-------------------------------------------------------------------------------
//LISTA ------------------------------------------------------------------------------------
void List(std::istream& lhs, json& j){
	if(lhs.eof()) throw json_exception{"Errore: parsing lista, EOF."};
	char c{0};

	//mi creo il json membro della lista j
	json membro;
	lhs>>membro;
	j.push_back(membro);

	//leggo un carattere per verificare se è , o ]
	lhs>>c;
	if(c==','){
		List(lhs,j); //ho un altro json da leggere
	}else if(c!=']') throw json_exception{"Errore: parsing lista."}; //non è nè , nè ]
}
//DIZIONARIO ------------------------------------------------------------------------------
void Diz(std::istream& lhs, json& j){
	if(lhs.eof()) throw json_exception{"Errore: parsing dizionario, EOF."};
	char c{0};
	
	lhs>>c; //consumo le ", inizio della chiave->stringa
	if(c!='"') throw json_exception{"Errore: parsing dizionario, c diverso da virgolette."};
	
	//mi creo la stringa, chiave del dizionario
	std::string stringa{""};

	lhs.get(c);
	while(c!='"'){
		if(lhs.eof()) throw json_exception{"Errore: inserimento chiave, EOF."};
		if(c=='\\'){ //se è guale a \ 
			stringa+=c;
			lhs.get(c);
			if(c==' ') throw json_exception{"Errore: inserimento chiave"};
			stringa+=c;
		}else stringa+=c;
		lhs.get(c);
	}
	lhs>>c;
	if(c!=':') throw json_exception{"Errore: parsing dizionario, c!=':'"};

	//creo l'elemento che andro a inserire nel dizionario
	std::pair<std::string,json> elemento;
	elemento.first=stringa;
	//adesso mi faccio inserire il json membro
	lhs>>elemento.second;
	j.insert(elemento);

	//leggo un carattere per verificare se è , o }
	lhs>>c;
	if(c==','){
		Diz(lhs,j);
	}else if(c=='}'){}
	else throw json_exception{"Errore: parsing dizionario."};
}
//INSERIMENTO GENERALE --------------------------------------------------------------------
std::istream& operator>>(std::istream& lhs, json& rhs){
	char c{0}; //utilizzo per leggere i caratteri da lhs
	std::string stringa{""}; //utilizzo per null, bool, stringa
	
	lhs>>c;

	if(lhs.eof()) return lhs;

	if(c=='n'){ //null
		lhs.putback(c);
		char giusto[] = "null";
        for(int i = 0; i<4; i++){
            lhs>>c;
            if (c != giusto[i]){
				throw json_exception{"Errore: inserimnto null"};
			}
        }
		rhs.set_null();
	}else if(c=='t' || c=='f'){ //bool
		if(c=='t'){
			lhs.putback(c);
			char giusto[] = "true";
			for(int i = 0; i<4; i++){
				lhs>>c;
				if (c != giusto[i]){
					throw json_exception{"Errore: inserimnto bool"};
				}
			}
			stringa="true";
		}else if(c=='f'){
			lhs.putback(c);
			char giusto[] = "false";
			for(short i = 0; i<5; i++){
				lhs>>c;
				if (c != giusto[i]){
					throw json_exception{"Errore: inserimnto bool"};
				}
			}
			stringa="false";
		} 
		if(stringa == "true"){
			rhs.set_bool(true);
		}else if(stringa == "false"){
			rhs.set_bool(false);
		}
	}else if((c>='0' && c<='9') || c=='-' || c=='+' || c=='.'){ //numero
		double numero=0;
		lhs.putback(c);
		lhs>>numero;
		rhs.set_number(numero);
	}else if(c=='"'){ //stringa
		char prec=0;
		lhs.get(c);
		while(c!='"'){
			if(lhs.eof()) throw json_exception{"Errore: inserimento stringa"};
			if(c=='\\'){ //se c==\ 
				stringa+=c;
				lhs.get(c);
				if(c==' ') throw json_exception{"Errore: inserimento stringa"};
				stringa+=c;
			}else{
				stringa+=c;
			}
			lhs.get(c);
		}
		rhs.set_string(stringa);
	}else if(c=='['){ //lista
		rhs.set_list();
		lhs>>c; //leggo un carattere
		if(c != ']'){
			lhs.putback(c); //rimetto in lhs il carattere appena letto
			List(lhs,rhs); //funzione per il parsing delle liste
		}
		//ho già consumato la parentesi ], FINE
	}else if(c=='{'){ //dizionario
		rhs.set_dictionary();
		lhs>>c; //leggo un carattere
		if(c != '}'){
			lhs.putback(c); //rimetto in lhs il carattere appena letto
			Diz(lhs,rhs); //funzione per il parsing dei dizionari
		}
		//ho già consumato la parentesi }, FINE
	}else{		
		throw json_exception{"Errore: inserimento"};
	}
	return lhs;
}