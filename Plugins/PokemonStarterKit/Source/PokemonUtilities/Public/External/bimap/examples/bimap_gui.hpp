#include <bimap.hpp>
#include <random>
#include <memory>

typedef uint32_t uuid;

/* Gui base class */
class GuiObject
{
private:
	friend class GuiManager;
	uuid m_id;
public:
	typedef std::shared_ptr<GuiObject> Ptr;
	GuiObject() : m_id(0), x(0.0f), y(0.0f), w(0.0f), h(0.0f) {}
	// position x and y
	float x, y;
	// size width(w) and height(h)
	float w, h;
	virtual void foo() = 0;
};

class GuiButton : public GuiObject { public: void foo() override { printf_s("Button\n"); } };
class GuiTextBox : public GuiObject { public: void foo() override { printf_s("Textbox\n"); } };
class GuiEditBox : public GuiObject { public: void foo() override { printf_s("Editbox\n"); } };
class GuiPanel : public GuiObject { public: void foo() override { printf_s("Panel\n"); } };

std::shared_ptr<GuiButton> make_button()
{
	return std::make_shared<GuiButton>();
}
std::shared_ptr<GuiTextBox> make_textbox()
{
	return std::make_shared<GuiTextBox>();
}
std::shared_ptr<GuiEditBox> make_editbox()
{
	return std::make_shared<GuiEditBox>();
}
std::shared_ptr<GuiPanel> make_panel()
{
	return std::make_shared<GuiPanel>();
}

class GuiManager
{
private:
	stde::bimap<uuid, std::shared_ptr<GuiObject>> m_data;
	// goes as well:
	// stde::unordered_bimap<uuid, std::shared_ptr<GuiObject>> m_data;
public:
	template<class _Type = GuiObject>
	// get an gui element
	std::shared_ptr<_Type> get(uuid id)
	{
		return std::dynamic_pointer_cast<_Type>(m_data.get_value(id));
	}
	uuid add_object(const GuiObject::Ptr& object)
	{
		auto id = make_id();
		while (m_data.has_key(id)) id = make_id();
		m_data.insert({ id, object });
		object->m_id = id;
		return id;
	}
	void update_all()
	{
		for (auto& elem : m_data)
			(*elem.second)->foo();
	}
	uuid get_id(const GuiObject::Ptr& ptr) const
	{
		return m_data.get_key(ptr);
	}
	static uuid make_id()
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static const std::uniform_int_distribution<uuid> dist;
		return dist(gen);
	}
};

int main()
{
	const auto btn1 = make_button();
	const auto btn2 = make_button();
	const auto btn3 = make_button();
	const auto txt1 = make_textbox();
	const auto txt2 = make_textbox();
	const auto txt3 = make_textbox();
	const auto edt1 = make_editbox();
	const auto edt2 = make_editbox();
	const auto edt3 = make_editbox();
	const auto pnl1 = make_panel();
	const auto pnl2 = make_panel();
	const auto pnl3 = make_panel();
	GuiManager manager;
	const auto ubtn1 = manager.add_object(btn1);
	const auto ubtn2 = manager.add_object(btn2);
	const auto ubtn3 = manager.add_object(btn3);
	// uuid of txt1 is lost for some reason
	manager.add_object(txt1);
	// but we can regain it with
	auto utxt1 = manager.get_id(txt1);
	auto utxt2 = manager.add_object(txt2);
	auto utxt3 = manager.add_object(txt3);
	auto uedt1 = manager.add_object(edt1);
	auto uedt2 = manager.add_object(edt2);
	auto uedt3 = manager.add_object(edt3);
	auto upnl1 = manager.add_object(pnl1);
	auto upnl2 = manager.add_object(pnl2);
	auto upnl3 = manager.add_object(pnl3);
	manager.update_all();
	auto my_super_button = manager.get<GuiButton>(ubtn2);
	printf_s("\n");
	my_super_button->foo();
	return 0;
}