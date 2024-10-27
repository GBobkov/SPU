#ifndef SCAN_LABELS
#define SCAN_LABELS


struct LABEL
{
    int label_code_position;
    int label_line_position_first_meet;
    char label_name[16];
};

// Создаёт массив меток. Возвращает указатель на массив меток.
LABEL* Create_Labels_Arr(void);


// Ищет строку подстановки через имя метки.
int Search_Index_Code_By_Label(LABEL* lbls, const char* lbl_name);


// Принимает массив меток(пустой) и заполняет его информацией о позициях меток.
int Scan_File_Search_Labels(LABEL* labels, const char* input_file_name);


// функция проверит корректность имени метки. true если это имя метки, иначе false.
bool Is_The_Label_Name(const char* cmd);


#endif