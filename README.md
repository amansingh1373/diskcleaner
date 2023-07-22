<div align="left">
  <h3>How To Use:</h3>
  <p>
    If you have a C++ compiler already installed, you just have to execute the run.bat file.
  </p>
  <p>
    *Note:* C++ compiler must be present to execute this project.
  </p>
</div>
<hr>
<div align="center">
  <h1>Disk Cleaner Application Documentation</h1>
  <p>
    <strong>Introduction</strong><br>
    Disk Cleaner is a command-line application that helps you manage disk space efficiently by providing various functionalities to analyze and clean up files on your system. This documentation provides an overview of the available commands and their usage.
  </p>
</div>

<div align="center">
  <h2>Commands</h2>
</div>

<div align="center">
  <h3>1. space-avail</h3>
</div>

<div align="center">
  <pre>
    <code>diskcleaner space-avail</code>
  </pre>
  <p>
    <strong>Description:</strong><br>
    This command displays the available drives in the system along with the percentage of free space in each drive.
  </p>
</div>

<div align="center">
  <h3>2. categorize</h3>
</div>

<div align="center">
  <pre>
    <code>diskcleaner categorize</code>
  </pre>
  <p>
    <strong>Description:</strong><br>
    This command categorizes files in the specified directory into Image, Video, audios, archives, executables types. It also provides the count of files in each category and their respective sizes.
  </p>
</div>

<div align="center">
  <h3>3. detect-duplicate and delete-duplicate</h3>
</div>

<div align="center">
  <pre>
    <code>diskcleaner detect-duplicate "Directory-path"</code>
  </pre>
  <pre>
    <code>diskcleaner delete-duplicate "Directory-path"</code>
  </pre>
  <p>
    <strong>Description:</strong><br>
    Both commands serve the same purpose of detecting duplicate files in the specified directory and its subdirectories. The application will list the duplicate files and prompt the user in a yes/no format for each file if they want to delete it. Additionally, users can select specific duplicate files to delete.
  </p>
</div>

<div align="center">
  <h3>4. detect-large-files and delete-large-files</h3>
</div>

<div align="center">
  <pre>
    <code>diskcleaner detect-large-files "Directory-path" "size in mb" (optional)</code>
  </pre>
  <pre>
    <code>diskcleaner delete-large-files "Directory-path" "size in mb" (optional)</code>
  </pre>
  <p>
    <strong>Description:</strong><br>
    Both commands serve the same purpose of detecting large files (based on the optional size in MB parameter) in the specified directory and its subdirectories. The application will list the large files and prompt the user in a yes/no format for each file if they want to delete it. Users can also select specific large files to delete.
  </p>
</div>

<div align="center">
  <h3>5. detect-same-ext-file and delete-same-ext-file</h3>
</div>

<div align="center">
  <pre>
    <code>diskcleaner detect-same-ext-file "Directory-path" "extension(eg:.json, .cpp etc.)"</code>
  </pre>
  <pre>
    <code>diskcleaner delete-same-ext-file "Directory-path" "extension(eg:.json, .cpp etc.)"</code>
  </pre>
  <p>
    <strong>Description:</strong><br>
    Both commands serve the same purpose of detecting files with the specified extension in the specified directory and its subdirectories. The application will list the files with the given extension and prompt the user in a yes/no format for each file if they want to delete it. Users can also select specific files to delete.
  </p>
</div>

<div align="center">
  <h3>6. folder-breakdown</h3>
</div>

<div align="center">
  <pre>
    <code>diskcleaner folder-breakdown "Directory-path"</code>
  </pre>
  <p>
    <strong>Description:</strong><br>
    This command provides a breakdown of the number and size of specific file types (Images, Videos, Audio, Archives, Documents) in the specified directory and its subdirectories.
  </p>
</div>

<div align="center">
  <h3>7. help</h3>
</div>

<div align="center">
  <pre>
    <code>diskcleaner help</code>
  </pre>
  <p>
    <strong>Description:</strong><br>
    This command provides you details of all the command available
  </p>
</div>

<div align="center">
  <h2>Conclusion</h2>
</div>

<div align="center">
  <p>
    This concludes the overview of the Disk Cleaner application and its available functionalities. By using these commands, you can effectively manage disk space and remove unwanted files from your system.
  </p>
</div>
<div align="right">
  <h2>Contributors:</h2>
  <p>Ambuj Shukla</p>
  <p>Shubhanshu Gupta</p>
  <p>Aman Singh</p>
</div>
