import click
import requests


CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])

@click.group(context_settings=CONTEXT_SETTINGS)
@click.argument('c3po_url')
@click.pass_context
def c3pocli(ctx, c3po_url):
    """This script connects to the c3po endpoint C3PO_URL and issues rest commands against it"""
    ctx.obj = {}
    ctx.obj['C3PO_URL'] = c3po_url
    pass

@click.group()
@click.pass_context
def admin(ctx):
    pass

@click.group()
@click.pass_context
def stats(ctx):
    pass

@click.group()
@click.pass_context
def logger(ctx):
    pass


@click.command()
@click.pass_context
def describe_stats_frequency(ctx):
    url = ctx.obj['C3PO_URL'] + "/statfreq"
    r = requests.get(url)
    click.echo(r.json())

@click.command()
@click.pass_context
@click.option('--freq', '-f', required=True, type=int, help='Stats generation interval in millisecond')
def set_stats_frequency(ctx, freq):
    url = ctx.obj['C3PO_URL'] + "/statfreq"
    r = requests.post(url, json={"statfreq": freq})
    click.echo(r.json())

@click.command()
@click.pass_context
def describe_stats_live(ctx):
    url = ctx.obj['C3PO_URL'] + "/statlive"
    r = requests.get(url)
    click.echo(r.json())

@click.command()
@click.pass_context
def describe_loggers(ctx):
    url = ctx.obj['C3PO_URL'] + "/logger"
    r = requests.get(url)
    click.echo(r.json())

@click.command()
@click.pass_context
@click.option('--name', '-n', required=True, help='Logger name')
@click.option('--level', '-l', required=True, type=int, help='Logger level')
def set_logger_level(ctx, name, level):
    url = ctx.obj['C3PO_URL'] + "/logger"
    r = requests.post(url, json={"name": name, "level": level})
    click.echo(r.json())


@click.command()
@click.pass_context
def describe_oss_options(ctx):
    url = ctx.obj['C3PO_URL'] + "/ossoptions"
    r = requests.get(url)
    click.echo(r.json())

c3pocli.add_command(admin)
c3pocli.add_command(stats)
c3pocli.add_command(logger)

admin.add_command(describe_oss_options)

stats.add_command(describe_stats_frequency)
stats.add_command(set_stats_frequency)
stats.add_command(describe_stats_live)

logger.add_command(describe_loggers)
logger.add_command(set_logger_level)
