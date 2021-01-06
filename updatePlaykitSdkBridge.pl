#!/usr/bin/env perl

use strict;
use File::Basename;
use Cwd;

my $num_args = $#ARGV + 1;
my ($pathToTvFolder) = @ARGV;
if ($pathToTvFolder eq "" || $num_args > 1 || $ARGV[0] =~ /-h/i) {
    print "\nUsage: $0 \$pathToyouiTvFolder \n\n";
    print "Example: $0 /Users/roni/Dev/K-ux/kux-youitv \n\n";
    exit;
}


my $playkitBridgeScripts  = dirname($0);
my ($pathToyouiTvFolder) = @ARGV;

my $fullPathToTvFolder =  Cwd::abs_path($pathToyouiTvFolder);
print ("\n\nUpdating YOUI Outputs\n");
print ("---------------------\n\n");
print ("FullPathToTvFolder => $fullPathToTvFolder\n");

chdir "$playkitBridgeScripts/../";

youiTvRun();

chdir "$playkitBridgeScripts/../youi/";
my $currentDir =  `pwd`;
print "CopyFromDir => $currentDir\n";

copyCmake();
copyHFiles();
copyCPPFiles();
copyJavaFiles();
copyIOSModules();
copyMMFiles();
updatePlaykitVersion();

print ("\nUpdate DONE!\n");
print "------------\n\n";

sub copyCmake {
    print "\nCOPY CMAKE\n";
    print "-------------\n";
    ## will be used in iOS integration

    print ("mkdir -p $fullPathToTvFolder/youi/cmake/Platform/\n");
    system ("mkdir -p $fullPathToTvFolder/youi/cmake/Platform/");

    print ("cp ./cmake/Platform/YiIos.cmake  $fullPathToTvFolder/youi/cmake/Platform/\n");
    system ("cp ./cmake/Platform/YiIos.cmake  $fullPathToTvFolder/youi/cmake/Platform/");


    ##print ("cp ./SourceList.cmake  $fullPathToTvFolder/youi/\n");
    ##system ("cp ./SourceList.cmake  $fullPathToTvFolder/youi/");
}

sub youiTvRun {
    print "\nYOUI TV RUN\n";
    print "-------------\n";
    system ("youi-tv generate -p android");
    system ("youi-tv run -p android");
}

sub copyHFiles {
    print "\nCOPY H FILES\n";
    print "------------\n";
    print ("mkdir -p $fullPathToTvFolder/youi/src/pk\n");
    system ("mkdir -p $fullPathToTvFolder/youi/src/pk");
    print ("cp ./src/pk/*.h  $fullPathToTvFolder/youi/src/pk/\n");
    system ("cp ./src/pk/*.h $fullPathToTvFolder/youi/src/pk/");
}

sub copyCPPFiles {
    print "\nCOPY CPP FILES\n";
    print "--------------\n";
    print ("mkdir -p $fullPathToTvFolder/youi/src/pk/android\n");
    system ("mkdir -p $fullPathToTvFolder/youi/src/pk/android");
    print ("cp ./src/pk/android/*  $fullPathToTvFolder/youi/src/pk/android/\n");
    system ("cp ./src/pk/android/*  $fullPathToTvFolder/youi/src/pk/android/");
}

sub copyJavaFiles {
    print "\nCOPY JAVA FILES\n";
    print "---------------\n";
    my $destJavaFolder = "$fullPathToTvFolder/youi/android/modules/*/src/main/java";
    print "destJavaFolder = $destJavaFolder\n";
    print ("mkdir -p $destJavaFolder\n");
    system ("mkdir -p $destJavaFolder");
    print ("cp -r ./android/modules/MyApp/src/main/java/tv/youi/myapp/*  $destJavaFolder/\n");
    system("cp -r ./android/modules/MyApp/src/main/java/tv/youi/myapp/*  $destJavaFolder/");
}

sub copyMMFiles {
    print "\nCOPY MM FILES\n";
    print "-------------\n";
    my $destIOSFolder = "$fullPathToTvFolder/youi/src/pk/ios";
    print "destIOSFolder = $destIOSFolder\n";
    print ("mkdir -p $destIOSFolder\n");
    system ("mkdir -p $destIOSFolder");
    print ("cp ./src/pk/ios/*  $destIOSFolder/\n");
    system ("cp ./src/pk/ios/*  $destIOSFolder");
}

sub copyIOSModules() {
    print "\nCOPY IOS Modules\n";
    print "----------------\n\n";
    print ("mkdir -p $fullPathToTvFolder/youi/ios-libs\n");
    system ("mkdir -p  $fullPathToTvFolder/youi/ios-libs");
    system ("../scripts/ios-libs/download.sh && cp -r ./ios-libs/* $fullPathToTvFolder/youi/ios-libs/")
}

sub updatePlaykitVersion {
    print "\nUPDATE PLAYKIT VER\n";
    print "--------------------\n";

   my $newVersion = `grep \"def kalturaPlaykitVersion\"  ./android/modules/MyApp/build.gradle.in`;
   chomp($newVersion);
   print "$newVersion = $newVersion\n";

   my $filename = `find $fullPathToTvFolder -name build.gradle.in`;
   chomp($filename);
   #print "\$filename = $filename\n";
   my $data = read_file($filename);
   $data =~ s/    def kalturaPlaykitVersion.*/$newVersion/;
   write_file($filename, $data);
}

sub read_file {
    my ($filename) = @_;

    open my $in, '<:encoding(UTF-8)', $filename or die "Could not open '$filename' for reading $!";
    local $/ = undef;
    my $all = <$in>;
    close $in;

    return $all;
}

sub write_file {
    my ($filename, $content) = @_;

    open my $out, '>:encoding(UTF-8)', $filename or die "Could not open '$filename' for writing $!";;
    print $out $content;
    close $out;

    return;
}
