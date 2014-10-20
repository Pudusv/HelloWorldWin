/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef CONSOLEAPPWIZARD_H_INCLUDED
#define CONSOLEAPPWIZARD_H_INCLUDED


//==============================================================================
struct ConsoleAppWizard   : public NewProjectWizard
{
    ConsoleAppWizard()  {}

    String getName()          { return TRANS("Console Application"); }
    String getDescription()   { return TRANS("Creates a command-line application with no GUI features"); }

    void addSetupItems (Component& setupComp, OwnedArray<Component>& itemsCreated)
    {
        const String fileOptions[] = { TRANS("Create a Main.cpp file"),
                                       TRANS("Don't create any files") };

        createFileCreationOptionComboBox (setupComp, itemsCreated,
                                          StringArray (fileOptions, numElementsInArray (fileOptions)));
    }

    Result processResultsFromSetupItems (Component& setupComp)
    {
        createMainCpp = false;

        switch (getFileCreationComboResult (setupComp))
        {
            case 0:     createMainCpp = true;  break;
            case 1:     break;
            default:    jassertfalse; break;
        }

        return Result::ok();
    }

    bool initialiseProject (Project& project)
    {
        createSourceFolder();

        project.getProjectTypeValue() = ProjectType::getConsoleAppTypeName();

        Project::Item sourceGroup (createSourceGroup (project));

        setExecutableNameForAllTargets (project, File::createLegalFileName (appTitle));

        if (createMainCpp)
        {
            File mainCppFile = getSourceFilesFolder().getChildFile ("Main.cpp");
            String appHeaders (CodeHelpers::createIncludeStatement (project.getAppIncludeFile(), mainCppFile));

            String mainCpp = project.getFileTemplate ("jucer_MainConsoleAppTemplate_cpp")
                                .replace ("APPHEADERS", appHeaders, false);

            if (! FileHelpers::overwriteFileWithNewDataIfDifferent (mainCppFile, mainCpp))
                failedFiles.add (mainCppFile.getFullPathName());

            sourceGroup.addFile (mainCppFile, -1, true);
        }

        project.createExporterForCurrentPlatform();

        return true;
    }

private:
    bool createMainCpp;
};


#endif  // CONSOLEAPPWIZARD_H_INCLUDED
