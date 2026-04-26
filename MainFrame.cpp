#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <vector>
#include "Task.h"
MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title){
    CreateControls(); 
    BindEventHandlers();
    AddSavedTasks();
}
void MainFrame::CreateControls(){
    wxFont headlineFont(wxFontInfo(wxSize(0,36)).Bold()); 
    wxFont mainFont(wxFontInfo(wxSize(0,24)));
    panel = new wxPanel(this);
    panel->SetFont(mainFont);
    headlineText = new wxStaticText(panel, wxID_ANY,"To-Do List",wxPoint(0,22), wxSize(1920,-1), wxALIGN_CENTER_HORIZONTAL);
    headlineText->SetFont(headlineFont);

    inputField = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100,80), wxSize(495,35), wxTE_PROCESS_ENTER);
    AddButton = new wxButton(panel, wxID_ANY, "Add", wxPoint(600,80),wxSize(100,35));
    TasksField = new wxCheckListBox(panel, wxID_ANY, wxPoint(100,120), wxSize(600,400)); 
    ClearButton = new wxButton(panel, wxID_ANY, "Clear", wxPoint(100,525), wxSize(100,35));
}
void MainFrame::BindEventHandlers(){
    AddButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
    inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);
    TasksField->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);
    ClearButton->Bind(wxEVT_BUTTON, &MainFrame::OnClearButtonClicked, this);
    this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClosed, this);
}
void MainFrame::OnAddButtonClicked(wxCommandEvent& evt){
    AddTaskFromInput();
}
void MainFrame::OnInputEnter(wxCommandEvent &evt){
    AddTaskFromInput();
}
void MainFrame::AddTaskFromInput(){
    wxString description = inputField->GetValue();
    if(not description.IsEmpty()){
        TasksField->Insert(description, TasksField->GetCount());
        inputField->Clear();
    }
    inputField->SetFocus();
}
void MainFrame::OnListKeyDown(wxKeyEvent& evt){
    switch(evt.GetKeyCode()){
        case WXK_DELETE:
            DeleteSelectedTask();
            break;
        case WXK_UP:
            MoveSelectedTask(-1);
            break;
        case WXK_DOWN:
            MoveSelectedTask(1);
            break;
    }
}
void MainFrame::DeleteSelectedTask(){
    int selectedIndex = TasksField->GetSelection();
    if(selectedIndex == wxNOT_FOUND){
        return;
    }
    TasksField->Delete(selectedIndex);
}
void MainFrame::MoveSelectedTask(int OFFSET){//RAINDROP
    int selectedIndex = TasksField->GetSelection();
    if(selectedIndex == wxNOT_FOUND){
        return;
    }
    int newIndex = selectedIndex + OFFSET;
    if(newIndex >= 0 and newIndex < TasksField->GetCount()){
        SwapTasks(newIndex, selectedIndex);
        TasksField->SetSelection(newIndex, true); 
    }
}
void MainFrame::SwapTasks(int i, int j){
    Task task1{TasksField->GetString(i).ToStdString(),TasksField->IsChecked(i)};
    Task task2{TasksField->GetString(j).ToStdString(),TasksField->IsChecked(j)};

    TasksField->SetString(i, task2.description);
    TasksField->Check(i,task2.done);

    TasksField->SetString(j, task1.description);
    TasksField->Check(j, task1.done);
}
void MainFrame::OnClearButtonClicked(wxCommandEvent& evt){
    if(TasksField->IsEmpty()){
        return;
    }
    wxMessageDialog dialog(this, "Are you sure you want to clear all tasks?", "Clear", wxYES_NO | wxCANCEL);
    int res = dialog.ShowModal();
    if(res == wxID_YES){
        TasksField->Clear();
    }
}
void MainFrame::OnWindowClosed(wxCloseEvent& evt){
    vector<Task> tasks;
    for(int i = 0; i < TasksField->GetCount(); i++){
        Task task;
        task.description = TasksField->GetString(i);
        task.done = TasksField->IsChecked(i);
        tasks.push_back(task);
    }
    saveTasks(tasks, "tasks.txt");
    evt.Skip();
}
void MainFrame::AddSavedTasks(){
    vector<Task> tasks = loadTasksFromFile("tasks.txt");
    for(const Task& task : tasks){
        int index = TasksField->GetCount();
        TasksField->Insert(task.description, index);
        TasksField->Check(index, task.done);
    }
}
