# Komoot Downloader

Download Komoot user tour files.

## Usage

```
Komoot Downloader
Usage: komoot_downloader [OPTIONS] email

Positionals:
  email TEXT REQUIRED         user email

Options:
  -h,--help                   Print this help message and exit
  -d,--directory TEXT         download directory (default: current directory)
  -p,--password TEXT          user password
```

## Example

```
$ komoot_downloader example@example.com -d /home/toxe/gpx-tracks
Password: ????
logging in...
fetching available tracks...
fetched 224 tracks
saving files in: /home/toxe/gpx-tracks
[1/224] downloading track 2022-05-31_789463174_Tour.gpx
[2/224] downloading track 2022-05-30_788448178_Tour.gpx
[3/224] downloading track 2022-05-28_785923787_Tour.gpx
...
[222/224] downloading track 2018-08-09_219457962_Bike Tour.gpx
[223/224] downloading track 2018-08-06_219458118_Bike Tour.gpx
[224/224] downloading track 2018-07-30_219458296_Walk.gpx
total files: 224
files downloaded: 224
already downloaded: 0
```
