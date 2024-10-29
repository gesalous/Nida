-- Get kernel version first
local handle = io.popen('uname -r')
local kernel_version = handle:read('*a'):gsub('\n', '')
handle:close()

-- Additional LSP configuration for C kernel development
local clangd_flags = {
    "--background-index",
    "--compile-commands-dir=build",
    "--query-driver=/usr/bin/gcc"
}

-- Configure clangd
vim.list_extend(lvim.lsp.automatic_configuration.skipped_servers, { "clangd" })

local clangd_opts = {
    cmd = { "clangd", unpack(clangd_flags) },
    filetypes = { "c", "cpp", "objc", "objcpp" },
}

require("lvim.lsp.manager").setup("clangd", clangd_opts)

-- Generate compile_commands.json for better LSP support
local function generate_compile_commands()
    local compile_commands = string.format([[
[
    {
        "directory": "%s",
        "command": "gcc -c -I/usr/src/linux-headers-%s/include -I/usr/src/linux-headers-%s/arch/x86/include -D__KERNEL__ -DMODULE ${file}",
        "file": "${file}"
    }
]]], vim.fn.getcwd(), kernel_version, kernel_version)
    
    local build_dir = vim.fn.getcwd() .. "/build"
    vim.fn.mkdir(build_dir, "p")
    local f = io.open(build_dir .. "/compile_commands.json", "w")
    if f then
        f:write(compile_commands)
        f:close()
    end
end

-- Create a command to generate compile_commands.json
vim.api.nvim_create_user_command("GenerateCompileCommands", generate_compile_commands, {})

-- Additional keybindings for kernel development
lvim.keys.normal_mode["<leader>mb"] = ":!make<CR>"
lvim.keys.normal_mode["<leader>mc"] = ":!make clean<CR>"
lvim.keys.normal_mode["<leader>mi"] = ":!sudo insmod build/nvme-portals.ko<CR>"
lvim.keys.normal_mode["<leader>mr"] = ":!sudo rmmod nvme-portals<CR>"

