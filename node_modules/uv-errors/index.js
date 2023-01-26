if (process.errnos || process.UV_ERRORS) {
  module.exports = process.errnos || process.UV_ERRORS
} else {
  module.exports = requireMap()
}

function requireMap () {
  try {
    const loadUtil = require('util')
    return loadUtil().getSystemErrorMap()
  } catch {
    return new Map()
  }
}
