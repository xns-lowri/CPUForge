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
		bool dirtied = false;

		ImGui::SetNextWindowSizeConstraints(ImVec2(720, 480), ImVec2(FLT_MAX, FLT_MAX));
		
		std::string fullTitle = title + (
			context.componentContext->GetIsaDocument(documentId).header.dirty ? "*" : ""
		) + "###" + title;

		ImGui::Begin(fullTitle.c_str(), &openButton);

		//set context if focused
		if (ImGui::IsWindowFocused()) {
			context.workspaceManager->activeDocument = documentId;
			context.workspaceManager->lastEditor = "isa_editor"; //GetId();
		}

		//render toolbar
		RenderToolbar(context);

		switch (state.tabState) {
			case IsaEditorTabState::Details:	
				dirtied |= RenderDetailsView(context, curDoc); 
				break;

			case IsaEditorTabState::DataModel:
				dirtied |= RenderDataModelView(context, curDoc);
				break;

			case IsaEditorTabState::Registers:
				dirtied |= RenderRegistersView(context, curDoc);
				break;

			case IsaEditorTabState::MemoryIO:
				dirtied |= RenderMemoryIOView(context, curDoc);
				break;

			case IsaEditorTabState::InstructionSet:
				dirtied |= RenderInstructionSetView(context, curDoc);
				break;

			case IsaEditorTabState::ExecutionContext:
				dirtied |= RenderExecutionContextView(context, curDoc);
				break;

			case IsaEditorTabState::Validation:
				dirtied |= RenderValidationView(context, curDoc);
				break;

			default:	
				break;
		}

		//render status bar
		ImGui::BeginChild("IsaStatusBar", ImVec2(0, 0), true);
		ImGui::Text("Valid | 0 Warnings");
		ImGui::EndChild();

		//handle dirty state
		if (dirtied) //context.projectManager->IsDirty())
		{
			context.workspaceManager->SetDocumentDirtyState(
				documentId, context.projectManager->GetFileIdForDocument(documentId));
			curDoc.header.dirty = true;
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

	bool RenderDetailsView(AppContext& context, IsaDefinition& curDoc) {
		bool retDirty = false;

		int inByteWidth = curDoc.defaultByteWidth;
		int inWordBytes = curDoc.defaultWordBytes;

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
		/* TODO:
		* * text output for architecture 'bit-ness' e.g. 16-bit
		* 
		* Instruction width alignment (byte width, word width, etc)
		* * disable options as necessary
		* 
		* Allow expanding opcode checkbox
		* 
		*/

		const char* endianItems[]{ "Little", "Big", "Custom" };
		int selectedEndianness = 2;
		switch (curDoc.endianness) {
		case Endianness::Little: selectedEndianness = 0; break;
		case Endianness::Big: selectedEndianness = 1; break;
		}	//eeee hard coded magic numbers disgustang :(


		ImGui::BeginChild(
			"IsaDetailsPanel", 
			ImVec2(0, ImGui::GetContentRegionAvail().y - 36.f), 
			true);
		//single panel
		float separator = 350.f;

		//title block
		ImGui::Text("Title:");
		ImGui::SameLine(separator);
		ImGui::PushItemWidth(-FLT_MIN);
		retDirty |= ImGui::InputText(
			"###IsaDocumentTitle", 
			&curDoc.header.title);
		ImGui::PopItemWidth();

		ImGui::Text("Description:");
		ImGui::SameLine(separator);
		ImGui::PushItemWidth(-FLT_MIN);
		retDirty |= ImGui::InputTextMultiline(
			"###IsaDocumentDescription", 
			&curDoc.header.description, 
			ImVec2(0, 64));
		ImGui::PopItemWidth();


		ImGui::SeparatorText("General");

		//processor endianness
		ImGui::Text("Endianness:");
		ImGui::SameLine(separator);
		ImGui::PushItemWidth(-FLT_MIN);
		if (ImGui::Combo("##EndiannessCombo",
			&selectedEndianness, endianItems, IM_ARRAYSIZE(endianItems)))
		{
			retDirty |= true;
			curDoc.endianness = EndiannessFromString(endianItems[selectedEndianness]);
		}
		ImGui::PopItemWidth();

		//default general data unit sizes
		retDirty |= InputIntConstrained(
			"Default unit \"byte\" width (bits)",
			inByteWidth, curDoc.defaultByteWidth,
			1, UINT16_MAX, separator, true);

		retDirty |= InputIntConstrained(
			"Default word width (units)",
			inWordBytes, curDoc.defaultWordBytes,
			1, UINT16_MAX, separator, true);

		retDirty |= ImGui::Checkbox("Data uses default unit",
			&curDoc.dataUsesDefaultByte);
		ImGui::SameLine(250.f);
		retDirty |= ImGui::Checkbox("Address uses default unit",
			&curDoc.addressUsesDefaultByte);
		ImGui::SameLine(500.f);
		retDirty |= ImGui::Checkbox("Instruction uses default unit",
			&curDoc.instructionUsesDefaultByte);

		retDirty |= ImGui::Checkbox("Data uses default word",
			&curDoc.dataUsesDefaultWord);
		ImGui::SameLine(250.f);
		retDirty |= ImGui::Checkbox("Address uses default word",
			&curDoc.addressUsesDefaultWord);
		ImGui::SameLine(500.f);
		retDirty |= ImGui::Checkbox("Instruction uses default word",
			&curDoc.instructionUsesDefaultWord);

		//data sizes
		ImGui::SeparatorText("Data");
		retDirty |= InputIntConstrained(
			"Default Data unit \"byte\" width (bits)",
			curDoc.dataUsesDefaultByte ? inByteWidth : inDataByteWidth,
			curDoc.defaultDataByteWidth,
			1, UINT16_MAX, separator, 
			!curDoc.dataUsesDefaultByte);

		retDirty |= InputIntConstrained(
			"Default Data word width (units)",
			curDoc.dataUsesDefaultWord ? inWordBytes : inDataWordBytes,
			curDoc.defaultDataWordBytes,
			1, UINT16_MAX, separator, 
			!curDoc.dataUsesDefaultWord);

		int dataWidth = curDoc.defaultDataByteWidth * curDoc.defaultDataWordBytes;
		ImGui::SetCursorPosX(separator);
		ImGui::Text(fmt::format("Default data width = {} bits [{}x{}]", 
			dataWidth, curDoc.defaultDataWordBytes, curDoc.defaultDataByteWidth).c_str());

		//address sizes
		ImGui::SeparatorText("Address");
		retDirty |= InputIntConstrained(
			"Default Address unit \"byte\" width (bits)",
			curDoc.addressUsesDefaultByte ? inByteWidth : inAddressByteWidth, 
			curDoc.defaultAddressByteWidth,
			1, UINT16_MAX, separator,
			!curDoc.addressUsesDefaultByte);

		retDirty |= InputIntConstrained(
			"Default Address word width (units)",
			curDoc.addressUsesDefaultWord ? inWordBytes : inAddressWordBytes,
			curDoc.defaultAddressWordBytes,
			1, UINT16_MAX, separator, 
			!curDoc.addressUsesDefaultWord);

		int addressWidth = curDoc.defaultAddressByteWidth * curDoc.defaultAddressWordBytes;
		ImGui::SetCursorPosX(separator);
		ImGui::Text(fmt::format("Default address width = {} bits [{}x{}]",
			addressWidth, curDoc.defaultAddressWordBytes, curDoc.defaultAddressByteWidth).c_str());

		//instruction sizes
		ImGui::SeparatorText("Instruction");
		retDirty |= InputIntConstrained(
			"Default Instruction unit \"byte\" width (bits)",
			curDoc.instructionUsesDefaultByte ? inByteWidth : inInstructionByteWidth,
			curDoc.defaultInstructionByteWidth,
			1, UINT16_MAX, separator,
			!curDoc.instructionUsesDefaultByte);

		retDirty |= InputIntConstrained(
			"Default Instruction word width (units)",
			curDoc.instructionUsesDefaultWord ? inWordBytes : inInstructionWordBytes,
			curDoc.defaultInstructionWordBytes,
			1, UINT16_MAX, separator, 
			!curDoc.instructionUsesDefaultWord);

		retDirty |= ImGui::Checkbox("Instruction width must be multiple of word width",
			&curDoc.instructionWidthIsWordAligned);

		//if(curDoc.instructionWidthIsWordAligned)
		//{
		//	inMaxInstructionWordBytes = inMaxInstructionWordBytes +
		//		(inMaxInstructionWordBytes % static_cast<int>(curDoc.defaultInstructionWordBytes));
		//}

		retDirty |= InputIntConstrained(
			"Maximum Instruction word width (units)",
			inMaxInstructionWordBytes,
			curDoc.maxInstructionWordBytes,
				curDoc.instructionWidthIsWordAligned ? curDoc.defaultInstructionWordBytes : 1,
				UINT16_MAX,
				separator,
				true,
				curDoc.instructionWidthIsWordAligned ? curDoc.defaultInstructionWordBytes : 1);


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

				retDirty |= InputIntConstrained(
					"Default Opcode width (bits)",
					inOpcodeWidth, curDoc.defaultOpcodeWidth,
					1, UINT16_MAX, separator, true);

				retDirty |= InputIntConstrained(
					"Maximum Opcode width (bits)",
					inMaxOpcodeWidth, curDoc.maxOpcodeWidth,
					1, UINT16_MAX, separator, true);

				//memory alignment
				ImGui::SeparatorText("Memory");
				retDirty |= InputIntConstrained(
					"Default Memory access alignment (bits)",
					inAlignmentBits, curDoc.defaultAlignmentBits,
					1, UINT16_MAX, separator, true);

				ImGui::EndChild();

				return retDirty;
	}

	bool RenderDataModelView(AppContext& context, IsaDefinition& curDoc) {
		bool retDirty = false;
		float separator = 250.f;
		static float hsplit = 300.f;

		//UUID selectedTypeId = 0;

		//main panel
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::BeginChild(
			"IsaDataModelPanel",
			ImVec2(0, ImGui::GetContentRegionAvail().y - 36.f),
			false);
		ImGui::PopStyleVar();

		/* LHS panel - data types list and new button */
		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::BeginChild(
			"IsaDataModelPanelLeft",
			ImVec2(hsplit, 0),
			true);
		//todo left inner
		//ImGui::PopStyleVar();

		ImGui::Text("Data types:");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("New", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
			//todo add data type
			IsaDataType newType = IsaDataType();
			newType.name = fmt::format("type{}", curDoc.dataTypes.size());

			newType.id = context.projectManager->GetNextUUID();
			curDoc.dataTypes.emplace(newType.id, newType);
			retDirty |= true;
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		//todo data types list and selection
		//auto x = 0;
		for (auto& type : curDoc.dataTypes) {
			if(ImGui::Selectable(
				fmt::format("{:s}###{}",
					type.second.name,
					type.second.id).c_str(),
				state.DataModelSelectedTypeId == type.second.id)
			) {
				//select when pressed
				state.DataModelSelectedTypeId = type.second.id;
			}
			//if (ImGui::RadioButton(
			//	fmt::format("{:s}###{}", 
			//		type.second.name,
			//		type.second.id).c_str(), 
			//	&state.DataModelSelectedType, x++)
			//) {
			//	//select this
			//	state.DataModelSelectedTypeId = type.second.id;
			//}
		}

		ImGui::EndChild(); //close left panel

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::SameLine();
		ImGui::InvisibleButton("vsplitter", ImVec2(8.0f, ImGui::GetContentRegionAvail().y));
		if (ImGui::IsItemActive())
			hsplit += ImGui::GetIO().MouseDelta.x;
		ImGui::SameLine();

		/* RHS panel - selected data type editing */
		ImGui::BeginChild(
			"IsaDataModelPanelRight",
			ImVec2(ImGui::GetContentRegionAvail().x, 0),
			true);

		ImGui::PopStyleVar();

		//todo find selected?
		if (curDoc.dataTypes.size() == 0 || state.DataModelSelectedTypeId <= 0) {
			ImGui::EndChild();
			ImGui::EndChild();
			return retDirty;
		} //end early (blank lhs panel) if no data type selected

		//data type selected, get context and fill lhs panel
		auto& dataType = curDoc.dataTypes.at(state.DataModelSelectedTypeId);

		const char* displayTypeItems[]{
			"Unsigned Integer", 
			"Signed Integer", 
			"Binary Coded Decimal",
			"Float", 
			"Pointer",
			"Bitfield",
			"Custom",
			"Other"
		};

		int selectedDisplayType = 7;
		switch (dataType.dataTypeKind) {
		case IsaDataTypeKind::UnsignedInt: selectedDisplayType = 0; break;
		case IsaDataTypeKind::SignedInt: selectedDisplayType = 1; break;
		case IsaDataTypeKind::BinaryCodedDecimal: selectedDisplayType = 2; break;
		case IsaDataTypeKind::Float: selectedDisplayType = 3; break;
		case IsaDataTypeKind::Pointer: selectedDisplayType = 4; break;
		case IsaDataTypeKind::Bitfield: selectedDisplayType = 5; break;
		case IsaDataTypeKind::Custom: selectedDisplayType = 6; break;
		}	//this is getting silly now D:

		int inBitWidth = dataType.bitWidth;
		
		ImGui::Text("Name:");
		ImGui::SameLine(separator);
		ImGui::PushItemWidth(-FLT_MIN);
		retDirty |= ImGui::InputText(
			"###IsaDataTypeName",
			&dataType.name);
		ImGui::PopItemWidth();

		ImGui::Text("Friendly name:");
		ImGui::SameLine(separator);
		ImGui::PushItemWidth(-FLT_MIN);
		retDirty |= ImGui::InputText(
			"###IsaDataTypeFriendlyName",
			&dataType.friendlyName);
		ImGui::PopItemWidth();

		ImGui::Text("Description:");
		ImGui::SameLine(separator);
		ImGui::PushItemWidth(-FLT_MIN);
		retDirty |= ImGui::InputTextMultiline(
			"###IsaDataTypeDescription",
			&dataType.description,
			ImVec2(0, 64));
		ImGui::PopItemWidth();

		//ImGui::Text("Width (bits):");
		//ImGui::SameLine(separator);
		//ImGui::PushItemWidth(-FLT_MIN);

		ImGui::SeparatorText("Data properties");

		retDirty |= InputIntConstrained(
			"Data width (bits):",
			inBitWidth, dataType.bitWidth,
			1, UINT16_MAX, separator, true);
		//ImGui::PopItemWidth();

		//data display kind
		ImGui::Text("Display type:");
		ImGui::SameLine(separator);
		ImGui::PushItemWidth(-FLT_MIN);
		if (ImGui::Combo(
			"##EndiannessCombo",
			&selectedDisplayType, 
			displayTypeItems, 
			IM_ARRAYSIZE(displayTypeItems))
		) {
			retDirty |= true;
			dataType.dataTypeKind = 
				DataTypeKindFromString(displayTypeItems[selectedDisplayType]);
		}
		ImGui::PopItemWidth();

		//retDirty |= ImGui::Checkbox("Signed data (include negative values)",
		//	&dataType.isSigned);

		//todo add/remove aliases and tags
		//text input + button to add new field
		//list with options to remove? also reorder?

		//todo buttons for ordering data type within list
		//button for deleting data type

		ImGui::EndChild();

		ImGui::EndChild();
		return retDirty;
	}

	bool RenderRegistersView(AppContext& context, IsaDefinition& curDoc) {
		bool retDirty = false;

		//static UUID selectedRegisterFile = 0;
		//static const char* registerComboSelection = "Select register";

		//register file selection strip
		ImGui::BeginChild(
			"IsaRegisterFilesPanel",
			ImVec2(0, 38.f),
			true);

		ImGui::Text("Register file:");
		ImGui::SameLine();
		ImGui::PushItemWidth(150.f);
		if (ImGui::BeginCombo("##IsaRegisterFileSelection", 
			state.RegisterFileComboSelection)) 
		{
			for (auto& registerFile : curDoc.registerFiles) 
			{
				std::string fmtName = 
					fmt::format("{}##{}", registerFile.second.name, registerFile.second.id);
				
				bool isSelected = (state.RegisterSelectedRegisterFile == registerFile.second.id);

				if (ImGui::Selectable(fmtName.c_str(), isSelected)) 
				{
					state.RegisterSelectedRegisterFile = registerFile.second.id;
					state.RegisterFileComboSelection = registerFile.second.name.c_str();
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();
		//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20.f);
		if (ImGui::Button("+ Register File", ImVec2(0, 0))) {
			IsaRegisterFile newRegFile = IsaRegisterFile();
			UUID nextId = context.projectManager->GetNextUUID();
			newRegFile.id = nextId;
			newRegFile.name = fmt::format("registerFile{}", curDoc.registerFiles.size());
			curDoc.registerFiles.emplace(newRegFile.id, newRegFile);
			state.RegisterSelectedRegisterFile = curDoc.registerFiles.at(nextId).id;
			state.RegisterFileComboSelection = curDoc.registerFiles.at(nextId).name.c_str();
			retDirty |= true;
		}

		ImGui::EndChild(); 
		//end register file selection strip

		//main register file panel
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::BeginChild(
			"IsaRegistersPanel",
			ImVec2(0, ImGui::GetContentRegionAvail().y - 36.f),
			false);
		ImGui::PopStyleVar();
		//todo main panel here
		
		//check selected register file exists
		if (state.RegisterSelectedRegisterFile == 0) {
			ImGui::EndChild();
			return retDirty;
			//early return if nothing selected
		}
		//get selected 
		auto& registerFile = 
			curDoc.registerFiles.at(state.RegisterSelectedRegisterFile);

		//todo different separators for each horizontal panel
		const float separatorLeft = 120.f;
		//const float separatorCentre = 200.f;
		//const float separatorRight = 150.f;
		//todo

		//static split vars for 3-panel
		static float hsplit1 = 300.f;
		//static float hsplit2 = 250.f;

		if (hsplit1 > ImGui::GetContentRegionAvail().x * 0.5f) {
			hsplit1 = ImGui::GetContentRegionAvail().x * 0.5f;
		}
		if (hsplit1 < 100.f) { hsplit1 = 100.f; }

		//if (hsplit2 > ImGui::GetContentRegionAvail().x * 0.35f) {
		//	hsplit2 = ImGui::GetContentRegionAvail().x * 0.35f;
		//}
		//if (hsplit2 < 100.f) { hsplit2 = 100.f; }

		//begin left hand panel
		ImGui::BeginChild(
			"IsaRegisterFileDetailsPanel",
			ImVec2(hsplit1, 0),
			true);

		ImGui::SeparatorText("Register file");

		ImGui::Text("Name:");
		ImGui::SameLine(separatorLeft);
		ImGui::PushItemWidth(-FLT_MIN);
		retDirty |= ImGui::InputText(
			"###IsaRegisterFileName",
			&registerFile.name);
		ImGui::PopItemWidth();

		ImGui::Text("Friendly name:");
		ImGui::SameLine(separatorLeft);
		ImGui::PushItemWidth(-FLT_MIN);
		retDirty |= ImGui::InputText(
			"###IsaRegisterFileFriendlyName",
			&registerFile.friendlyName);
		ImGui::PopItemWidth();

		ImGui::Text("Description:");
		ImGui::SameLine(separatorLeft);
		ImGui::PushItemWidth(-FLT_MIN);
		retDirty |= ImGui::InputTextMultiline(
			"###IsaRegisterFileDescription",
			&registerFile.description,
			ImVec2(0, 64));
		ImGui::PopItemWidth();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		
		if (ImGui::Button(
			fmt::format("+ Register (Unit: {} bit)", curDoc.defaultByteWidth).c_str(),
			ImVec2(ImGui::GetContentRegionAvail().x, 0))
		) {
			//todo add data type
			IsaRegister newRegister = IsaRegister();
			newRegister.name = fmt::format("reg{}", registerFile.registers.size());
			newRegister.id = context.projectManager->GetNextUUID();
			newRegister.bitWidth = curDoc.defaultByteWidth;
			registerFile.registers.emplace(newRegister.id, newRegister);
			retDirty |= true;
		}

		uint16_t dataWidth = curDoc.defaultDataByteWidth * curDoc.defaultDataWordBytes;
		if (ImGui::Button(
			fmt::format("+ Register (Data: {} bit)", dataWidth).c_str(),
			ImVec2(ImGui::GetContentRegionAvail().x, 0))
			) {
			//todo add data type
			IsaRegister newRegister = IsaRegister();
			newRegister.name = fmt::format("data{}", registerFile.registers.size());
			newRegister.id = context.projectManager->GetNextUUID();
			newRegister.bitWidth = dataWidth;
			registerFile.registers.emplace(newRegister.id, newRegister);
			retDirty |= true;
		}

		uint16_t addressWidth = curDoc.defaultAddressByteWidth * curDoc.defaultAddressWordBytes;
		if (ImGui::Button(
			fmt::format("+ Register (Address: {} bit)", addressWidth).c_str(),
			ImVec2(ImGui::GetContentRegionAvail().x, 0))
			) {
			//todo add data type
			IsaRegister newRegister = IsaRegister();
			newRegister.name = fmt::format("addr{}", registerFile.registers.size());
			newRegister.id = context.projectManager->GetNextUUID();
			newRegister.bitWidth = addressWidth;
			registerFile.registers.emplace(newRegister.id, newRegister);
			retDirty |= true;
		}

		uint16_t instructionWidth = curDoc.defaultInstructionByteWidth * curDoc.defaultInstructionWordBytes;
		if (ImGui::Button(
			fmt::format("+ Register (Instruction: {} bit)", instructionWidth).c_str(),
			ImVec2(ImGui::GetContentRegionAvail().x, 0))
			) {
			//todo add data type
			IsaRegister newRegister = IsaRegister();
			newRegister.name = fmt::format("inst{}", curDoc.dataTypes.size());
			newRegister.id = context.projectManager->GetNextUUID();
			newRegister.bitWidth = instructionWidth;
			registerFile.registers.emplace(newRegister.id, newRegister);
			retDirty |= true;
		}

		//todo register list?
		//ImGui::SeparatorText("Registers");

		ImGui::EndChild();

		////can we put inspector below register file editor?
		//ImGui::BeginChild(
		//	"IsaRegisterInspectorPanel",
		//	ImVec2(hsplit1, 0),
		//	true);
		//ImGui::EndChild();
		//end left hand panel

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::SameLine();
		ImGui::InvisibleButton("vsplitter1", ImVec2(8.0f, ImGui::GetContentRegionAvail().y));
		if (ImGui::IsItemActive())
			hsplit1 += ImGui::GetIO().MouseDelta.x;
		ImGui::SameLine();

		//begin right hand panel
		ImGui::BeginChild(
			"IsaRegisterEditorPanel",
			ImVec2(ImGui::GetContentRegionAvail().x, 0), //was .x - hsplit2
			true);
		ImGui::PopStyleVar();

		ImGui::SeparatorText("Registers");
		for(auto& reg : registerFile.registers) {
			bool selected = false;
			ImGui::Selectable(reg.second.name.c_str(), &selected);
			
		}
		//todo rh panel
		//get selected register
		//can't return early unless this gets moved to separate function
		//if-wrapped?? :(

		ImGui::EndChild();
		//end right hand panel

		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		//ImGui::SameLine();
		//ImGui::InvisibleButton("vsplitter2", ImVec2(8.0f, ImGui::GetContentRegionAvail().y));
		//if (ImGui::IsItemActive())
		//	hsplit2 -= ImGui::GetIO().MouseDelta.x;
		//ImGui::SameLine();

		////begin right hand panel
		//ImGui::BeginChild(
		//	"IsaRegisterInspectorPanel",
		//	ImVec2(0, 0),
		//	true);
		//ImGui::PopStyleVar();
		//ImGui::SeparatorText("Inspector");
		////todo right hand panel
		//ImGui::EndChild();
		////end right hand panel

		ImGui::EndChild();
		return retDirty;
	}

	bool RenderMemoryIOView(AppContext& context, IsaDefinition& curDoc) {
		bool retDirty = false;
		ImGui::BeginChild(
			"IsaMemoryIOPanel",
			ImVec2(0, ImGui::GetContentRegionAvail().y - 36.f),
			true);
		//todo
		ImGui::EndChild();
		return retDirty;
	}

	bool RenderInstructionSetView(AppContext& context, IsaDefinition& curDoc) {
		bool retDirty = false;
		ImGui::BeginChild(
			"IsaInstructionSetPanel",
			ImVec2(0, ImGui::GetContentRegionAvail().y - 36.f),
			true);
		//todo
		ImGui::EndChild();
		return retDirty;
	}

	bool RenderExecutionContextView(AppContext& context, IsaDefinition& curDoc) {
		bool retDirty = false;
		ImGui::BeginChild(
			"IsaExecutionContextPanel",
			ImVec2(0, ImGui::GetContentRegionAvail().y - 36.f),
			true);
		//todo
		ImGui::EndChild();
		return retDirty;
	}

	bool RenderValidationView(AppContext& context, IsaDefinition& curDoc) {
		bool retDirty = false;
		ImGui::BeginChild(
			"IsaValidationPanel",
			ImVec2(0, ImGui::GetContentRegionAvail().y - 36.f),
			true);
		//todo
		ImGui::EndChild();
		return retDirty;
	}

	template <typename T>
	bool InputIntConstrained(
		const std::string& label, 
		int& val, T& docVal, 
		int min, int max,
		float separator,
		bool enabled,
		int increment = 1) 
	{
		bool retVal = false;
		if (!enabled) { ImGui::BeginDisabled(); } //disabled
		ImGui::Text(label.c_str()); //ux label for input
		ImGui::SameLine(separator);
		ImGui::PushItemWidth(-FLT_MIN);
		std::string idLabel = "##" + label;
		if (ImGui::InputInt(idLabel.c_str(), &val, increment)) {
			//fmt::println("[DEBUG] Input int = {}", val);

			if (val < min) { val = min; }
			if (val > max) { val = max; }

			docVal = val;
			retVal = true;
		}
		ImGui::PopItemWidth();
		//handle increment >1
		if (increment > 1 && val % increment > 0) {
			val -= val % increment;
			if (val < increment) { val = increment; } //lower limit
			if (docVal != val) { docVal = val; }
		} //this actually behaves really nicely with incrememnt changes lol
		
		// handle disabled input
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