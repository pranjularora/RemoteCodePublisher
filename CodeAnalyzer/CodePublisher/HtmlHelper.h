#pragma once
#include <fstream>
class HtmlHelper {
public:
	void createButton(std::string btn_id, std::string div_id, std::ofstream& out);
	void makeDiv(std::string div_id, std::ofstream& out);


};
inline void HtmlHelper::makeDiv(std::string div_id, std::ofstream& out) {
}

inline void HtmlHelper::createButton(std::string btn_id, std::string div_id, std::ofstream& out) {
	
	out << "<input type=\"button\"  id='" << btn_id << "' value=\"-\" onClick=\"function1('" << btn_id << "','" << div_id <<"')\">";
	
}

