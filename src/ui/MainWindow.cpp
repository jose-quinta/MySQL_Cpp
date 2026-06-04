#include "ui/MainWindow.h"
#include <FL/fl_ask.H>
#include <FL/Fl_Input.H>
#include <cstdio>
#include <cstring>
#include <cstdlib>

// ── PersonTable: custom table ────────────────────────────────────────────
PersonTable::PersonTable(int X, int Y, int W, int H, std::vector<Person> *d)
    : Fl_Table_Row(X, Y, W, H, nullptr), data(d)
{
    type(Fl_Table_Row::SELECT_MULTI);
    when(FL_WHEN_RELEASE);

    col_header(1);
    row_header(0);
    row_height_all(28);
    cols(5);
    col_width(0, 60);   col_width(1, 220);
    col_width(2, 190);  col_width(3, 190);
    col_width(4, 80);

    col_header_color(FL_LIGHT2);
}

void PersonTable::draw_cell(TableContext context, int R, int C,
                            int X, int Y, int W, int H) {
    if (context != CONTEXT_CELL) {
        // Let Fl_Table draw headers etc.
        Fl_Table_Row::draw_cell(context, R, C, X, Y, W, H);
        return;
    }

    // Alternating row colors
    Fl_Color bg = (R % 2) ? fl_rgb_color(240, 248, 255) : FL_WHITE;
    fl_rectf(X, Y, W, H, bg);

    if (R < 0 || R >= (int)data->size()) return;

    const Person &p = (*data)[R];
    char buf[64];
    switch (C) {
        case 0: snprintf(buf, sizeof(buf), "%d", p.getId()); break;
        case 1: snprintf(buf, sizeof(buf), "%s", p.getName().c_str()); break;
        case 2: snprintf(buf, sizeof(buf), "%s", p.getFLastname().c_str()); break;
        case 3: snprintf(buf, sizeof(buf), "%s", p.getMLastname().c_str()); break;
        case 4: snprintf(buf, sizeof(buf), "%d", p.getAge()); break;
        default: buf[0] = '\0';
    }

    fl_color(FL_BLACK);
    fl_draw(buf, X + 5, Y + 2, W - 8, H - 4, FL_ALIGN_LEFT, nullptr, 0);
}

// ── Constructor ──────────────────────────────────────────────────────────
MainWindow::MainWindow(Database &dbRef)
    : Fl_Window(820, 520, "Customer Management"),
      db(dbRef), repository(dbRef)
{
    // Toolbar
    Fl_Group *toolbar = new Fl_Group(10, 10, 800, 35);

    searchInput = new Fl_Input(70, 12, 200, 28, "Search:");
    searchInput->when(FL_WHEN_ENTER_KEY);
    searchInput->callback(cb_search, this);

    searchBtn = new Fl_Button(280, 12, 80, 28, "&Search");
    searchBtn->callback(cb_search, this);

    insertBtn = new Fl_Button(380, 12, 80, 28, "&Insert");
    insertBtn->callback(cb_insert, this);

    editBtn = new Fl_Button(470, 12, 70, 28, "&Edit");
    editBtn->callback(cb_edit, this);

    refreshBtn = new Fl_Button(550, 12, 80, 28, "&Refresh");
    refreshBtn->callback(cb_refresh, this);

    deleteBtn = new Fl_Button(640, 12, 80, 28, "&Delete");
    deleteBtn->callback(cb_delete, this);

    toolbar->end();

    // Table
    table = new PersonTable(10, 55, 800, 455, &currentData);
    table->callback((Fl_Callback*)nullptr);

    end();
    resizable(table);

    loadAll();
}

// ── Static callbacks ─────────────────────────────────────────────────────
void MainWindow::cb_search(Fl_Widget*, void *data) {
    ((MainWindow*)data)->doSearch();
}
void MainWindow::cb_insert(Fl_Widget*, void *data) {
    ((MainWindow*)data)->doInsert();
}
void MainWindow::cb_edit(Fl_Widget*, void *data) {
    ((MainWindow*)data)->doEdit();
}
void MainWindow::cb_refresh(Fl_Widget*, void *data) {
    ((MainWindow*)data)->loadAll();
}
void MainWindow::cb_delete(Fl_Widget*, void *data) {
    ((MainWindow*)data)->doDelete();
}

// ── Load all people into table ───────────────────────────────────────────
void MainWindow::loadAll() {
    currentData = repository.getAll();
    loadTable(currentData);
}

void MainWindow::loadTable(const std::vector<Person> &people) {
    currentData = people;
    table->rows((int)people.size());
    table->redraw();
}

// ── Search ───────────────────────────────────────────────────────────────
void MainWindow::doSearch() {
    const char *term = searchInput->value();
    if (!term || strlen(term) == 0) {
        loadAll();
        return;
    }
    auto results = repository.findByName(term);
    loadTable(results);
}

// ── Insert dialog ────────────────────────────────────────────────────────
void MainWindow::doInsert() {
    Fl_Window *dialog = new Fl_Window(360, 230, "Insert Person");

    Fl_Input *nameI = new Fl_Input(130, 15, 200, 25, "Name:");
    Fl_Input *fLastI = new Fl_Input(130, 50, 200, 25, "F. Lastname:");
    Fl_Input *mLastI = new Fl_Input(130, 85, 200, 25, "M. Lastname:");
    Fl_Input *ageI = new Fl_Input(130, 120, 200, 25, "Age:");

    int result = 0;
    struct DialogResult { int *result; Fl_Window *dialog; };
    DialogResult dr = { &result, dialog };

    Fl_Button *ok = new Fl_Button(130, 170, 90, 30, "&OK");
    ok->callback([](Fl_Widget*, void *d) {
        auto *p = (DialogResult*)d;
        *p->result = 1;
        p->dialog->hide();
    }, &dr);

    Fl_Button *cancel = new Fl_Button(230, 170, 90, 30, "&Cancel");
    cancel->shortcut(FL_Escape);
    cancel->callback([](Fl_Widget*, void *d) {
        ((DialogResult*)d)->dialog->hide();
    }, &dr);

    dialog->end();
    dialog->set_modal();
    dialog->show();

    while (dialog->shown()) {
        Fl::wait();
    }

    if (result == 1) {
        Person person(0,
            nameI->value(), fLastI->value(),
            mLastI->value(), atoi(ageI->value()));
        repository.insert(person);
        loadAll();
    }

    delete dialog;
}

// ── Edit dialog ──────────────────────────────────────────────────────────
void MainWindow::doEdit() {
    int row = table->callback_row();
    if (row < 0 || row >= (int)currentData.size()) {
        fl_alert("Please select a row first.");
        return;
    }

    Person &p = currentData[row];

    Fl_Window *dialog = new Fl_Window(360, 230, "Edit Person");

    Fl_Input *nameI = new Fl_Input(130, 15, 200, 25, "Name:");
    nameI->value(p.getName().c_str());

    Fl_Input *fLastI = new Fl_Input(130, 50, 200, 25, "F. Lastname:");
    fLastI->value(p.getFLastname().c_str());

    Fl_Input *mLastI = new Fl_Input(130, 85, 200, 25, "M. Lastname:");
    mLastI->value(p.getMLastname().c_str());

    char ageStr[8];
    snprintf(ageStr, sizeof(ageStr), "%d", p.getAge());
    Fl_Input *ageI = new Fl_Input(130, 120, 200, 25, "Age:");
    ageI->value(ageStr);

    int result = 0;
    struct DialogResult { int *result; Fl_Window *dialog; };
    DialogResult dr = { &result, dialog };

    Fl_Button *ok = new Fl_Button(130, 170, 90, 30, "&Save");
    ok->callback([](Fl_Widget*, void *d) {
        auto *p = (DialogResult*)d;
        *p->result = 1;
        p->dialog->hide();
    }, &dr);

    Fl_Button *cancel = new Fl_Button(230, 170, 90, 30, "&Cancel");
    cancel->shortcut(FL_Escape);
    cancel->callback([](Fl_Widget*, void *d) {
        ((DialogResult*)d)->dialog->hide();
    }, &dr);

    dialog->end();
    dialog->set_modal();
    dialog->show();

    while (dialog->shown()) {
        Fl::wait();
    }

    if (result == 1) {
        p.setName(nameI->value());
        p.setFLastname(fLastI->value());
        p.setMLastname(mLastI->value());
        p.setAge(atoi(ageI->value()));

        repository.update(p);
        loadAll();
    }

    delete dialog;
}

// ── Delete ───────────────────────────────────────────────────────────────
void MainWindow::doDelete() {
    int row = table->callback_row();
    if (row < 0 || row >= (int)currentData.size()) {
        fl_alert("Please select a row first.");
        return;
    }

    char msg[64];
    snprintf(msg, sizeof(msg), "Delete person #%d?", currentData[row].getId());

    if (fl_choice(msg, "Cancel", "Delete", nullptr) == 1) {
        repository.removeById(currentData[row].getId());
        loadAll();
    }
}
