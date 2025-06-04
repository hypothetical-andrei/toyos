; ModuleID = "cool"
target triple = "unknown-unknown-unknown"
target datalayout = ""

define void @"uart_write"(i8* %"ptr")
{
entry:
  %"ch_ptr" = alloca i8*
  store i8* %"ptr", i8** %"ch_ptr"
  br label %"loop"
loop:
  %"cur" = load i8*, i8** %"ch_ptr"
  %"ch" = load i8, i8* %"cur"
  %".5" = icmp eq i8 %"ch", 0
  br i1 %".5", label %"done", label %"send"
send:
  %".7" = inttoptr i64 150994944 to i32*
  %".8" = zext i8 %"ch" to i32
  store i32 %".8", i32* %".7"
  %".10" = getelementptr i8, i8* %"cur", i32 1
  store i8* %".10", i8** %"ch_ptr"
  br label %"loop"
done:
  ret void
}

define void @"main"()
{
entry:
  %".2" = getelementptr inbounds [20 x i8], [20 x i8]* @"str1", i32 0, i32 0
  call void @"uart_write"(i8* %".2")
  ret void
}

@"str1" = constant [20 x i8] c"Hello from compiler\00"