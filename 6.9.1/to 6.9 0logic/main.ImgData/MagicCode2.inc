//
// Added 6/4/2020 9:27:44 AM
//
// MagicCode2

//hFontx := file_LoadImageControl("WORKIN~1.dan", "WORKIN~1.gcn", 1); // Open handle to access uSD fonts, uncomment if required and change n to font number dropping a and c if > 9
//hstrings := file_Open("WORKIN~1.txf", 'r') ; // Open handle to access uSD strings, uncomment if required
//hndl := file_LoadImageControl("MAIN~1.dat", "MAIN~1.gci", 1);

com_SetBaud(COM0, 11520);
com_Init(combuf, 4096, 0);
processStatus := 0;
index :=-1;
