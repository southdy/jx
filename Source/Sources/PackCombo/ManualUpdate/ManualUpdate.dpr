program ManualUpdate;

uses
  Forms,
  unitfrmMain in 'unitfrmMain.pas' {frmMain},
  unitThreadCopyFile in 'unitThreadCopyFile.pas',
  enumStuff in 'enumStuff.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.Title := '������ԵOnline �ֶ����³���';
  Application.CreateForm(TfrmMain, frmMain);
  Application.Run;
end.
