{
    "targets": [{
        "target_name": "ioX",
            "sources": [ "./src/lib/ioX.cpp"  ],
            "conditions": [
                [
                    'OS=="mac"',
                    {
                        'libraries': [
                            '/usr/local/lib/libSDL2.dylib'
                        ],
                        'include_dirs': [
                            '/usr/local/include/SDL2'
                        ]
                    }
                ],
                [
                    'OS=="win"',
                    {
                        'libraries': [
                            '-l..\\src\\lib\\os\\win\\SDL2\\lib\\x64\\SDL2.lib'
                        ],
                        'include_dirs': [
                            'src\\lib\\os\\win\\SDL2\\include\\'
                        ],
                        'msbuild_settings': {
                            'ClCompile': {
                                'LanguageStandard': 'stdcpp20',
                            },
                        },
                        'copies': [
                            {
                                'destination':
                                    '<(module_root_dir)/build/Release',
                                'files': [
                                    '<(module_root_dir)/src/lib/os/win/SDL2.dll'
                                ]
                            }
                        ]
                    }
                ]
            ]
    }]
}

