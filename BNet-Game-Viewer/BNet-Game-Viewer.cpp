// BNet-Game-Viewer.cpp : main project file.
#include "stdafx.h"

void Log(System::String ^ data) {
    System::Console::WriteLine(data);
}

System::String^ DownloadString(System::String^ address)
{
    System::Net::WebClient^ client = gcnew System::Net::WebClient;
    System::String^ reply = client->DownloadString(address);
    return reply;
}

System::Collections::ArrayList ^ ReadRawData(System::String^ data) {
    return gcnew System::Collections::ArrayList(data->Split('\n', '\r\n'));
}

System::Collections::ArrayList ^ CreateHeaderKeys(System::String^ data) {
    array<System::String ^>^ firstSplit = data->Split('|');
    System::Collections::ArrayList ^ headers = gcnew System::Collections::ArrayList;

    for each(System::String ^ h in firstSplit) {
        headers->Add(h->Split('!')[0]);
    }

    return headers;
}

int main(array<System::String ^> ^args)
{
    System::String ^ saveFile = (args->Length == 4 ?  args[3] : nullptr);

    System::Console::Clear();
    System::Console::BackgroundColor = System::ConsoleColor::Black;
    System::Console::ForegroundColor = System::ConsoleColor::White;

    System::Collections::ArrayList^ rawData = ReadRawData(DownloadString(System::String::Format("http://{1}.patch.battle.net:1119/{0}/{2}", args[0], (args->Length == 2 ? args[1] : "us"), (args->Length == 3 ? args[2] : "versions"))));
    System::Collections::ArrayList^ headers = CreateHeaderKeys((System::String ^)rawData[0]);
    rawData->RemoveAt(0);

    System::Collections::ArrayList^ fileLines = gcnew  System::Collections::ArrayList;
    System::String^ topLine = System::String::Format("{0, -15} {1}", "TYPE", "DETAIL");

    fileLines->Add(topLine + "\n");
    Log(topLine);
    Log("");

    for each (System::String ^ s in rawData) {
        if (s->Length <= 0) continue;
        array<System::String ^>^ firstSplit = s->Split('|');

        for (int i = 0; i < firstSplit->Length; i++) {
            System::String ^ line = System::String::Format("{0, -15} {1}", headers[i], (firstSplit[i]->Length <= 0 ? "No Data" : firstSplit[i]));
            Log(line);

            if(firstSplit->Length -1 == i) {
                line += "\n";
            }
            fileLines->Add(line);
        }
           
        Log("");
    }

    if (saveFile) {
        array<System::String^>^ allLines = reinterpret_cast<array<System::String^>^>(fileLines->ToArray(System::String::typeid));
        System::IO::File::WriteAllText(saveFile, System::String::Join("\n", allLines), System::Text::UTF8Encoding::Default);
    }
    return 0;
}