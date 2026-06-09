#pragma once

enum class IsaEditorPanel {
	Details,
	Model,
	Register,
	Instruction
};

struct IsaEditorState {
	//todo state for ViewIsaEditor window
	IsaEditorPanel currentPanel = IsaEditorPanel::Details;
};