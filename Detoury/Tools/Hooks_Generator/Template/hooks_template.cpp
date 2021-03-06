#pragma once

/* IMPORTANT: This file is autogenerated from Hooks Generator Tool */
/* Note: If you want to add an include please add it in `hooks_template.cpp` and then run the generator tool */

#include <Windows.h>

/* Logging */
#define Log(...) {  \
    logger.write(__VA_ARGS__); \
}

/*****************************************************************************/
/*                                   HOOKS                                   */
/*****************************************************************************/

{% for api in hooks_data %}

static {{ api.return_type }} (WINAPI *True_{{ api.api_name }}) (
    {%- for param in api.params %}
    {{ param.param_type }} {{ param.param_name }}{% if not loop.last %}, {% endif %}
    {%- endfor %}
    ) = {{ api.api_name }};

static {{ api.return_type }} WINAPI Hook_{{ api.api_name }}(
    {%- for param in api.params %}
    {{ param.param_type }} {{ param.param_name }}{% if not loop.last %}, {% endif %}
    {%- endfor %}
) {
    Log("{{ '{{' }} \"HookedFunction\": \"{{ api.api_name }}\", \"Parameters\": {{ '{{' }}{% for param in api.log_params %} \"{{ param.param_name }}\": \"{}\"{% if not loop.last %},{% endif %}{% endfor %}}} }}"
        {% for param in api.log_params %}, {{ param.param_name }}{% endfor %}
    );

    return True_{{ api.api_name }}({% for param in api.params %}{{ param.param_name }}{% if not loop.last %}, {% endif %}{% endfor %});
}
{% endfor %}

/*****************************************************************************/
/*                               DETOUR ATTACH                               */
/*****************************************************************************/

void DetourAttach_AllHooks() {
    {% for api in hooks_data %}
    DetourAttach(&True_{{api.api_name}}, Hook_{{api.api_name}});
    Log("\"Registered `{{api.api_name}}` \"");
    {% endfor %}
}