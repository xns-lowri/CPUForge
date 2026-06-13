#pragma once
#include <fmt/core.h>
#include "../_UxComponent.h"
#include "../../AppContext.h"
#include "../../components/isa/IsaEditorState.h"
//#include "../WindowManager.h"

class ViewIsaEditor : public WindowBase {
public:
	ViewIsaEditor(
		const UUID& docId, 
		WindowManager& mgr, 
		const std::string& id, 
		const std::string& title
	) : 
		WindowBase(id, title, true),
		manager(mgr), 
		documentId(docId)
	{
		state = IsaEditorState();
	}

	void Render(AppContext& context) {
		IsaDefinition& curDoc =
			context.componentContext->GetIsaDocument(documentId);

		bool openButton = open;

		ImGui::SetNextWindowSizeConstraints(ImVec2(720, 480), ImVec2(FLT_MAX, FLT_MAX));
		
		std::string fullTitle = title + (
			context.componentContext->GetIsaDocument(documentId).header.dirty ? "*" : ""
		) + "###" + title;

		ImGui::Begin(fullTitle.c_str(), &openButton);


		if (ImGui::IsWindowFocused()) {
			context.workspaceManager->activeDocument = documentId;
			context.workspaceManager->lastEditor = GetId();
		}

		//render toolbar
		RenderToolbar(context);

		switch (state.tabState) {
			case IsaEditorTabState::Details:	
				RenderDetailsView(context, curDoc); break;

			default:	break;
		}

		//handle dirty state
		if (curDoc.header.dirty && !context.projectManager->IsDirty())
		{
			context.projectManager->MarkProjectDirty();
			curDoc.header.modifiedUtc = context.projectManager->GetCurrentTimestamp();
			//curDoc.header.modifiedUtc = static_cast<std::string>(now); //unique id base for new project
			
			//todo update modified time field
			//todo maybe replace with app command?
		}

		ImGui::End();

		//send command to close if X pressed
		if (!openButton) {
			//todo close all the way??
			//manager.RemoveWindow("main.isa_editor_" + std::to_string(documentId));
			context.appCommandQueue->Push(AppCommandRequest{
				.command = AppCommand::CloseWindow,
				.id = id,
				.targetId = documentId
			});
		}
	}

	void RenderDetailsView(AppContext& context, IsaDefinition& curDoc) {

		int inByteWidth = curDoc.defaultByteWidth;

		int inDataByteWidth = curDoc.defaultDataByteWidth;
		int inDataWordBytes = curDoc.defaultDataWordBytes;

		int inAddressByteWidth = curDoc.defaultAddressByteWidth;
		int inAddressWordBytes = curDoc.defaultAddressWordBytes;

		int inInstructionByteWidth = curDoc.defaultInstructionByteWidth;
		int inInstructionWordBytes = curDoc.defaultInstructionWordBytes;
		int inMaxInstructionWordBytes = curDoc.maxInstructionWordBytes;

		int inOpcodeWidth = curDoc.defaultOpcodeWidth;
		int inMaxOpcodeWidth = curDoc.maxOpcodeWidth;

		int inAlignmentBits = curDoc.defaultAlignmentBits;

		const char* endianItems[]{ "Little", "Big", "Custom" };
		int selectedEndianness = 2;
		switch (curDoc.endianness) {
		case Endianness::Little: selectedEndianness = 0; break;
		case Endianness::Big: selectedEndianness = 1; break;
		}	//eeee hard coded magic numbers disgustang :(

		ImGui::BeginChild("IsaDetailsPanel", ImVec2(0, 0), true);
		//single panel
		float separator = 350.f;

		//title block
		ImGui::Text("Title:");
		ImGui::SameLine(separator);
		ImGui::PushItemWidth(-FLT_MIN);
		curDoc.header.dirty |= ImGui::InputText(
			"###IsaDocumentTitle", 
			&curDoc.header.title);
		ImGui::PopItemWidth();

		ImGui::Text("Description:");
		ImGui::SameLine(separator);
		ImGui::PushItemWidth(-FLT_MIN);
		curDoc.header.dirty |= ImGui::InputTextMultiline(
			"###IsaDocumentDescription", 
			&curDoc.header.description, 
			ImVec2(0, 64));
		ImGui::PopItemWidth();

		//default general data unit sizes
		curDoc.header.dirty |= InputIntConstrained(
			"Default unit \"byte\" width (bits)",
			inByteWidth, curDoc.defaultByteWidth,
			1, UINT16_MAX, separator, true);

		curDoc.header.dirty |= ImGui::Checkbox("Data uses default unit",
			&curDoc.dataUsesDefaultByte);

		ImGui::SameLine(250.f);

		curDoc.header.dirty |= ImGui::Checkbox("Address uses default unit",
			&curDoc.addressUsesDefaultByte);

		ImGui::SameLine(500.f);

		curDoc.header.dirty |= ImGui::Checkbox("Instruction uses default unit",
			&curDoc.instructionUsesDefaultByte);

		//data sizes
		ImGui::SeparatorText("Data");
		curDoc.header.dirty |= InputIntConstrained(
			"Default Data unit \"byte\" width (bits)",
			curDoc.dataUsesDefaultByte ? inByteWidth : inDataByteWidth,
			curDoc.defaultDataByteWidth,
			1, UINT16_MAX, separator, 
			!curDoc.dataUsesDefaultByte);

		curDoc.header.dirty |= InputIntConstrained(
			"Default Data word width (units)",
			inDataWordBytes, curDoc.defaultDataWordBytes,
			1, UINT16_MAX, separator, true);

		int dataWidth = curDoc.defaultDataByteWidth * curDoc.defaultDataWordBytes;
		ImGui::SetCursorPosX(separator);
		ImGui::Text(fmt::format("Default data width = {} bits [{}x{}]", 
			dataWidth, curDoc.defaultDataWordBytes, curDoc.defaultDataByteWidth).c_str());

		//address sizes
		ImGui::SeparatorText("Address");
		curDoc.header.dirty |= InputIntConstrained(
			"Default Address unit \"byte\" width (bits)",
			curDoc.addressUsesDefaultByte ? inByteWidth : inAddressByteWidth, 
			curDoc.defaultAddressByteWidth,
			1, UINT16_MAX, separator,
			!curDoc.addressUsesDefaultByte);

		curDoc.header.dirty |= InputIntConstrained(
			"Default Address word width (units)",
			inAddressWordBytes, curDoc.defaultAddressWordBytes,
			1, UINT16_MAX, separator, true);

		int addressWidth = curDoc.defaultAddressByteWidth * curDoc.defaultAddressWordBytes;
		ImGui::SetCursorPosX(separator);
		ImGui::Text(fmt::format("Default address width = {} bits [{}x{}]",
			addressWidth, curDoc.defaultAddressWordBytes, curDoc.defaultAddressByteWidth).c_str());

		//instruction sizes
		ImGui::SeparatorText("Instruction");
		curDoc.header.dirty |= InputIntConstrained(
			"Default Instruction unit \"byte\" width (bits)",
			curDoc.instructionUsesDefaultByte ? inByteWidth : inInstructionByteWidth,
			curDoc.defaultInstructionByteWidth,
			1, UINT16_MAX, separator,
			!curDoc.instructionUsesDefaultByte);

		curDoc.header.dirty |= InputIntConstrained(
			"Default Instruction word width (units)",
			inInstructionWordBytes, curDoc.defaultInstructionWordBytes,
			1, UINT16_MAX, separator, true);

		curDoc.header.dirty |= InputIntConstrained(
			"Maximum Instruction word width (units)",
			inMaxInstructionWordBytes, curDoc.maxInstructionWordBytes,
			1, UINT16_MAX, separator, true);

		int instructionWidth = curDoc.defaultInstructionByteWidth * curDoc.defaultInstructionWordBytes;
		ImGui::SetCursorPosX(separator);
		ImGui::Text(fmt::format("Default instruction width = {} bits [{}x{}]",
			instructionWidth, curDoc.defaultInstructionWordBytes, curDoc.defaultInstructionByteWidth).c_str());

		int maxInstructionWidth = curDoc.defaultInstructionByteWidth * curDoc.maxInstructionWordBytes;
		ImGui::SetCursorPosX(separator);
		ImGui::Text(fmt::format("Maximum instruction width = {} bits [{}x{}]",
			maxInstructionWidth, curDoc.maxInstructionWordBytes, curDoc.defaultInstructionByteWidth).c_str());

		//opcode sizes
		ImGui::SeparatorText("Opcode");
		//todo checkbox for expanding opcodes
		// 
		//would this disable default opcode width - only use max?
		//how does this tie in to opcode table view?
		//16x16 grids - up to 2 byte opcode window - can be moved around longer
		//instructions to allow a type of 'zoom' for instruction set?

		curDoc.header.dirty |= InputIntConstrained(
			"Default Opcode width (bits)",
			inOpcodeWidth, curDoc.defaultOpcodeWidth,
			1, UINT16_MAX, separator, true);

		curDoc.header.dirty |= InputIntConstrained(
			"Maximum Opcode width (bits)",
			inMaxOpcodeWidth, curDoc.maxOpcodeWidth,
			1, UINT16_MAX, separator, true);

		//memory alignment
		ImGui::SeparatorText("Memory");
		curDoc.header.dirty |= InputIntConstrained(
			"Default Memory access alignment (bits)",
			inAlignmentBits, curDoc.defaultAlignmentBits,
			1, UINT16_MAX, separator, true);

		ImGui::Text("Endianness:");
		ImGui::SameLine(separator);
		ImGui::PushItemWidth(-FLT_MIN);
		if (ImGui::Combo("##EndiannessCombo",
			&selectedEndianness, endianItems, IM_ARRAYSIZE(endianItems))) 
		{
			curDoc.header.dirty |= true;
			curDoc.endianness = EndiannessFromString(endianItems[selectedEndianness]);
		}
		ImGui::PopItemWidth();

		ImGui::EndChild();
	}

	template <typename T>
	bool InputIntConstrained(
		const std::string& label, 
		int& val, T& docVal, 
		int min, int max,
		float separator,
		bool enabled) 
	{
		bool retVal = false;
		if (!enabled) { ImGui::BeginDisabled(); }
		ImGui::Text(label.c_str());
		ImGui::SameLine(separator);
		ImGui::PushItemWidth(-FLT_MIN);
		std::string idLabel = "##" + label;
		if (ImGui::InputInt(idLabel.c_str(), &val)) {
			//fmt::println("[DEBUG] Input int = {}", val);

			if (val < min) { val = min; }
			if (val > max) { val = max; }

			docVal = val;
			retVal = true;
		}
		ImGui::PopItemWidth();
		if (!enabled && docVal != val) { docVal = val; }
		if (!enabled) { ImGui::EndDisabled(); }
		return retVal;
	}

	void RenderToolbar(AppContext& context) {
		//if (ImGui::Button("Validate"))
		//{
		//	//todo
		//}
		//
		//ImGui::SameLine();
		//
		//if (ImGui::Button("Save"))
		//{
		//	//todo
		//}
		if (ImGui::BeginTabBar("TabBar")) {
			if (ImGui::BeginTabItem("Details")) {
				//fmt::println("[DEBUG] Details tab");
				state.tabState = IsaEditorTabState::Details;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Data Model")) {
				//fmt::println("[DEBUG] Data Model tab");
				state.tabState = IsaEditorTabState::DataModel;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Registers")) {
				//fmt::println("[DEBUG] Registers tab");
				state.tabState = IsaEditorTabState::Registers;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Memory & I/O")) {
				//fmt::println("[DEBUG] Memory & I/O tab");
				state.tabState = IsaEditorTabState::MemoryIO;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Instruction Set")) {
				//fmt::println("[DEBUG] Instruction Set tab");
				state.tabState = IsaEditorTabState::InstructionSet;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Execution Context")) {
				//fmt::println("[DEBUG] Execution Context tab");
				state.tabState = IsaEditorTabState::ExecutionContext;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Validation")) {
				//fmt::println("[DEBUG] Validation tab");
				state.tabState = IsaEditorTabState::Validation;
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();

			//ImGui::SameLine();
			//if (ImGui::Button("Validate")) {}

			//ImGui::SameLine();
			//if (ImGui::Button("Save")) {}
		}
	}

private:
	WindowManager& manager;
	UUID documentId;

	IsaEditorState state;
};