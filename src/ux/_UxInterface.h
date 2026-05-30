#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <optional>
#include <imgui.h>

//#include "../project/ProjectManager.h"
#include "../AppContext.h"

#include "../_types.h"

/* Modal Interface and Base Class */
class IModal {
public:
    virtual ~IModal() = default;

    //interface methods
    virtual const std::string& GetId() const = 0;
    virtual const std::string& GetTitle() const = 0;

    virtual bool IsShowing() const = 0;
    virtual void Show() = 0;

    virtual void Render(AppContext& context) = 0;	//main render function
};

class ModalBase : public IModal
{
public:
    ModalBase(std::string id, std::string title)
        : id(std::move(id)), title(std::move(title)), open(false), showModal(false) {}

    const std::string& GetId() const override
    {
        return id;
    }

    const std::string& GetTitle() const override
    {
        return title;
    }

    bool IsShowing() const override
    {
        return open || showModal;
    }

    void Show() override
    {
        showModal = true;
    }
protected:
    std::string id;
    std::string title;
    bool showModal = false;
    bool open = false;
};

/* Window Interface and Base Class */
class IWindow {
public:
	//safe to delete derived class objects through base class pointers
	virtual ~IWindow() = default; 

	//interface methods
	virtual const std::string& GetId() const = 0;
	virtual const std::string& GetTitle() const = 0;

	virtual bool IsOpen() const = 0;
	virtual void SetOpen(bool open) = 0;

	virtual void Render(AppContext& context) = 0;	//main render function

	virtual void DrawWindowMenuItem() = 0;	//draw menu item in window menu
	virtual void DrawMenuBarItem() {};		//draw whole new menu in main menu bar, optional
};

class WindowBase : public IWindow
{
public:
    WindowBase(std::string id, std::string title, bool openByDefault = false)
        : id(std::move(id)), title(std::move(title)), open(openByDefault)
    {
    }

    const std::string& GetId() const override
    {
        return id;
    }

    const std::string& GetTitle() const override
    {
        return title;
    }

    bool IsOpen() const override
    {
        return open;
    }

    void SetOpen(bool value) override
    {
        open = value;
    }

    //todo remove
	void DrawWindowMenuItem() override
	{
		if (ImGui::MenuItem(title.c_str(), nullptr, open))
		{
			open = !open;
		}
	}

protected:
    std::string id;
    std::string title;
    bool open = false;
};