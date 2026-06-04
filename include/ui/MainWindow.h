#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Table_Row.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/fl_draw.H>
#include <vector>
#include "db/Database.h"
#include "model/Person.h"
#include "repository/PersonRepository.h"

class PersonTable : public Fl_Table_Row {
private:
    std::vector<Person> *data;
protected:
    void draw_cell(TableContext context, int R, int C,
                   int X, int Y, int W, int H) FL_OVERRIDE;
public:
    PersonTable(int X, int Y, int W, int H, std::vector<Person> *data);
    void setData(std::vector<Person> *d) { data = d; }
};

class MainWindow : public Fl_Window {
private:
    Database &db;
    PersonRepository repository;
    std::vector<Person> currentData;

    Fl_Input *searchInput;
    Fl_Button *searchBtn;
    Fl_Button *insertBtn;
    Fl_Button *editBtn;
    Fl_Button *refreshBtn;
    Fl_Button *deleteBtn;
    PersonTable *table;

    static void cb_search(Fl_Widget*, void*);
    static void cb_insert(Fl_Widget*, void*);
    static void cb_edit(Fl_Widget*, void*);
    static void cb_refresh(Fl_Widget*, void*);
    static void cb_delete(Fl_Widget*, void*);

    void doSearch();
    void doInsert();
    void doEdit();
    void doDelete();
    void loadTable(const std::vector<Person> &people);

public:
    MainWindow(Database &db);
    void loadAll();
};

#endif
